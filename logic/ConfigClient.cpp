#include "ConfigClient.h"
#include <iostream>
#include "Log.h"
#include <mutex>
#include "JsonConfig.h"
#include "JsonConfigNode.h"

#include "config_update.pb.h"
#include "gate.pb.h"
#include "msg_id.pb.h"
#include "notify.pb.h"
#include "Helper.h"
#include "err_code.pb.h"

#include <shlobj.h>
#include <filesystem>

#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp> // 假设使用 nlohmann JSON 库

namespace fs = std::filesystem;


ConfigClient::ConfigClient(const std::string& host, uint16_t port)
    : host_(host), port_(port), running_(false)
{
    tcpClient_ = std::make_unique<TcpClient>(host, port, &in_channel_, &out_channel_, &timer_);
    char path[512];
    if (!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
    {
        ELOG << "error to get appdata path";
        return;
    }

    // 构建完整路径: %APPDATA%\config_up\config.json
    std::filesystem::path config_dir = std::filesystem::path(path) / "config_up";
    std::filesystem::create_directories(config_dir);  // 确保目录存在

    std::filesystem::path config_path = config_dir / "config.json";
    std::string full_path = config_path.string();

    // 将配置保存在%APPDATA%/config_up/config.json中
    configMaker_.load(full_path);
}

ConfigClient::~ConfigClient()
{
    stop();
}

void ConfigClient::start() 
{
    running_ = true;

    if(0)
    {
        //用户选择开始处
Choice:
        std::cout << std::endl << std::endl << std::endl;
    }
    int32_t choice = inputLoop();

    handleUserChoice(choice);
    if(choice != 1)
    {
        // 如果不是网络相关的，则重新选择
        goto Choice;
    }

    if(running_)
    {
        input_thread_ = std::thread(&ConfigClient::heartLoop, this);
        receive_thread_ = std::thread(&ConfigClient::receiveLoop, this);
    }
}

void ConfigClient::stop() 
{
    if (!running_) return;
    running_ = false;

    if (input_thread_.joinable()) input_thread_.join();
    if (receive_thread_.joinable()) receive_thread_.join();

    tcpClient_->stop();
}

void ConfigClient::join() 
{
    if (input_thread_.joinable()) input_thread_.join();
    if (receive_thread_.joinable()) receive_thread_.join();
}

int32_t ConfigClient::inputLoop() 
{
    if (running_) 
    {
        std::cout << "请选择操作:"           << std::endl \
                    << "1. 更新配置"         << std::endl \
                    << "2. 重新打包"         << std::endl \
                    << "3. 添加仅更新的文件" << std::endl \
                    << "4. 删除仅更新的文件" << std::endl \
                    << "5. 清空所有文件配置" << std::endl \
                    << "6. 打开配置文件"     << std::endl \
                    << "请输入选项: ";
        int choice = 0;
        std::cin >> choice;
    
        // 清除输入缓冲区
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        return choice;
    }

    return -1;
}

// 获取项目根目录
fs::path getProjectRoot() {
    try {
        return fs::canonical(fs::path("/proc/self/exe")).parent_path();
    } catch (...) {
        return fs::current_path();
    }
}

// 处理文件路径函数
std::string processFilePath(const std::string& input) {
    fs::path inputPath(input);
    fs::path projectRoot = getProjectRoot();
    
    // 去除可能的引号
    std::string clean_input = input;
    if (!clean_input.empty() && clean_input.front() == '"' && clean_input.back() == '"') {
        clean_input = clean_input.substr(1, clean_input.size() - 2);
    }
    inputPath = fs::path(clean_input);
    
    // 处理相对路径
    if (inputPath.is_relative()) {
        fs::path fullPath = projectRoot / inputPath;
        
        if (!fs::exists(fullPath)) {
            throw std::runtime_error("文件不存在: " + fullPath.string());
        }
        
        return inputPath.generic_string();
    }
    // 处理绝对路径
    else {
        fs::path absPath = fs::canonical(inputPath);
        
        // 检查是否在项目目录内
        if (absPath.string().find(projectRoot.string()) != 0) {
            throw std::runtime_error("文件必须在项目目录内: " + projectRoot.string());
        }
        
        // 转换为相对路径
        fs::path relativePath = fs::relative(absPath, projectRoot);
        
        if (!fs::exists(absPath)) {
            throw std::runtime_error("文件不存在: " + absPath.string());
        }
        
        return relativePath.generic_string();
    }
}

void ConfigClient::handleUserChoice(int choice) 
{
    switch (choice) 
    {
        case 1: 
        case 2: 
        {
            std::shared_ptr<NetPack> pack = std::make_shared<NetPack>();
            pack->seq = 1;
            pack->len = 0;
            pack->flag = C2S_FLAG;
            
            cs::ConfigUpdate req;
            if (choice == 1) {
                auto& files = configMaker_["update_files"].asArray();
                for (const auto& file : files) 
                {
                    req.mutable_request()->add_update_file(file.value<std::string>());
                }
            } else {
                std::cout << "打包功能尚未实现" << std::endl;
                break;
            }
            
            pack->msg_id = MSGID::CS_CONFIG_UPDATE;
            req.SerializeToString(&pack->msg);
            
            tcpClient_->start();

            out_channel_.push({ 0, pack });
            break;
        }
        case 3: // 添加文件
        {
            std::cout << "\n当前文件列表:\n";
            for (size_t i = 0; i < configMaker_["update_files"].asArray().size(); ++i) {
                std::cout << "[" << i + 1 << "] " << Helper::utf8_to_gbk(configMaker_["update_files"].asArray()[i].value<std::string>()) << "\n";
            }
            
            std::cout << "若要返回，可直接回车\n\n"
                      << "请输入要添加的文件名（可以将文件拖入到窗口中直接回车）:\n"
                      << "示例: 运营/拍脸礼包.xlsx 或 C:/Users/yeyeye/Desktop/config/运营/拍脸礼包.xlsx\n"
                      << "请输入: ";
            
            std::string file_name;
            // getchar(); // 读取换行符
            std::getline(std::cin, file_name); // 使用 getline 支持带空格路径
            
            try {
                std::string processed_path = processFilePath(file_name);
                processed_path = Helper::gbk_to_utf8(processed_path);
                if(processed_path == "") break;
                
                // 检查是否已存在
                auto& files = configMaker_["update_files"];
                bool exists = false;
                for (const auto& file : files.asArray()) {
                    if (file.value<std::string>() == processed_path) {
                        exists = true;
                        break;
                    }
                }
                
                if (exists) {
                    std::cout << "文件已在列表中: " << processed_path << "\n";
                } else {
                    files.append(processed_path);
                    configMaker_.save();
                    std::cout << "已添加文件: " << processed_path << "\n";
                }
            } catch (const std::exception& e) {
                std::cout << "错误: " << e.what() << "\n";
            }
            break;
        }
        case 4: // 移除文件
        {
            std::cout << "移除文件功能尚未实现，请使用选项 5 进行清空或使用选项 6 打开配置文件手动编辑。\n";
            break;

            auto& files = configMaker_["update_files"];
            if (files.asArray().empty()) {
                std::cout << "文件列表为空，无文件可移除。\n";
                break;
            }
            
            std::cout << "\n当前文件列表:\n";
            for (size_t i = 0; i < files.asArray().size(); ++i) {
                std::cout << "[" << i + 1 << "] " << Helper::utf8_to_gbk(files.asArray()[i].value<std::string>()) << "\n";
            }
            
            std::cout << "\n请输入要移除的文件序号 (1-" << files.asArray().size() << ")，或输入 0 取消: ";
            int index;
            if (!(std::cin >> index)) {
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                std::cout << "无效输入。\n";
                break;
            }
            
            if (index == 0) {
                std::cout << "操作已取消。\n";
            } else if (index > 0 && static_cast<size_t>(index) <= files.asArray().size()) {
                std::string removed = files[index - 1].value<std::string>();


                // 删除功能尚未实现
                // files.erase(files.begin() + index - 1);


                configMaker_.save();
                std::cout << "已移除文件: " << removed << "\n";
            } else {
                std::cout << "无效序号。\n";
            }
            break;
        }
        case 5: // 清空文件列表
        {
            if (configMaker_["update_files"].asArray().empty()) {
                std::cout << "文件列表已为空。\n";
            } else {
                std::cout << "确定要清空所有文件吗? (y/n): ";
                char confirm;
                std::cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    configMaker_["update_files"] = nlohmann::json::array();
                    configMaker_.save();
                    std::cout << "已清空所有文件。\n";
                } else {
                    std::cout << "操作已取消。\n";
                }
            }
            break;
        }
        case 6: // 打开文件
        {
            std::string full_path = configMaker_.getPath();
            // 跨平台打开文件
            #ifdef _WIN32
            std::string command = "start \"\" \"" + full_path + "\"";
            #elif __APPLE__
            std::string command = "open \"" + full_path.string() + "\"";
            #else // Linux
            std::string command = "xdg-open \"" + full_path.string() + "\"";
            #endif
            
            int result = std::system(command.c_str());
            if (result != 0) {
                std::cerr << "无法打开文件。错误代码: " << result << "\n";
            }
            break;
        }
        default:
        {
            std::cout << "无效选项，请重新输入。\n";
            break;
        }
    }
}

void ConfigClient::receiveLoop() 
{
    while (running_) 
    {
        std::mutex pop_mutex;
        std::lock_guard<std::mutex> lock(pop_mutex);

        auto [conn_id, pack] = in_channel_.pop();
        handleServerResponse(pack);
    }
}

void ConfigClient::heartLoop() 
{
    while (running_) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        std::shared_ptr<NetPack> pack = std::make_shared<NetPack>();
        pack->seq = 1;
        pack->len = 0;
        pack->flag = C2S_FLAG;
        pack->msg_id = MSGID::CS_HEART_BEAT;

        Heart heart;
        heart.SerializeToString(&pack->msg);

        out_channel_.push({ 0, pack });
    }
}

void ConfigClient::handleServerResponse(const std::shared_ptr<NetPack>& pack) 
{
    switch (pack->msg_id) 
    {
        case MSGID::CS_HEART_BEAT: 
        {
            // ILOG  << "heart!";
            break;
        }
        case MSGID::CS_CONFIG_UPDATE: 
        {
            cs::ConfigUpdate resp;
            if (resp.ParseFromString(pack->msg)) 
            {
                if(resp.response().err() != ErrorCode::Error_success)
                {
                    std::cout << "err: " << ErrorCode::ErrorCode_Name(resp.response().err()) << "\n";
                }
                std::cout << "配置更新成功!" << "\n";
            } 
            break;
        }
        case MSGID::SC_NOTIFY:
        {
            cs::Notify notify;
            if (notify.ParseFromString(pack->msg))
            {
                std::string contextByGbk= Helper::utf8_to_gbk(notify.config_update_output().context());
                std::cout << contextByGbk << "\n";
            }
        }
    }
}
