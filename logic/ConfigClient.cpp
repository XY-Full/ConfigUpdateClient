#include "ConfigClient.h"
#include <iostream>
#include "Log.h"
#include <mutex>

#include "config_update.pb.h"
#include "gate.pb.h"
#include "msg_id.pb.h"
#include "notify.pb.h"
#include "err_code.pb.h"

ConfigClient::ConfigClient(const std::string& host, uint16_t port)
    : host_(host), port_(port), running_(false)
{
    tcpClient_ = std::make_unique<TcpClient>(host, port, &in_channel_, &out_channel_, &timer_);
}

ConfigClient::~ConfigClient()
{
    stop();
}

void ConfigClient::start() 
{
    running_ = true;

    int32_t choice = inputLoop();

    tcpClient_->start();

    ILOG << "start ok, running_: " << running_;

    handleUserChoice(choice);

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
        std::cout << "请选择操作:" << std::endl << "1. 更新配置" << std::endl << "2. 重新打包" << std::endl << " 请输入选项: ";
        int choice = 0;
        std::cin >> choice;

        return choice;
    }

    return -1;
}

void ConfigClient::handleUserChoice(int choice) 
{
    std::shared_ptr<NetPack> pack = std::make_shared<NetPack>();
    pack->seq = 1;
    pack->len = 0;
    pack->flag = C2S_FLAG;

    switch (choice) 
    {
        case 1: 
        {
            cs::ConfigUpdate req;
            req.mutable_request()->add_update_file("运营/拍脸礼包.xlsx");

            pack->msg_id = MSGID::CS_CONFIG_UPDATE;
            req.SerializeToString(&pack->msg);
            break;
        }
        case 2: 
        {
            cs::ConfigUpdate req;
            req.mutable_request()->add_update_file("pack");

            pack->msg_id = MSGID::CS_CONFIG_UPDATE;
            req.SerializeToString(&pack->msg);
            break;
        }
        default:
        {
            std::cout << "无效选项，请重新输入。\n";
            return;
        }
    }

    out_channel_.push({ 0, pack });
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
                std::cout << notify.config_update_output().context() << "\n";
            }
        }
    }
}
