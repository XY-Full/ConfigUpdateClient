#include "Helper.h"
#include "Log.h"
#include <iomanip>

#include <string>

void Helper::printStringData(std::string data)
{
    std::cout << "String content as bytes: ";
    for (size_t i = 0; i < data.size(); ++i) 
    {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)data[i] << " ";
    }
    std::cout << std::dec << std::endl;  // 恢复十进制输出
}

#ifdef _WIN32
#include <windows.h>
#include <vector>

// Windows 平台：从 GBK 转 UTF-8
std::string Helper::gbk_to_utf8(const std::string &gbk)
{
    if (gbk.empty()) return "";
    
    // 获取本地代码页
    UINT codePage = CP_ACP;
    
    // 计算所需宽字符缓冲区大小
    int wlen = MultiByteToWideChar(codePage, 0, gbk.c_str(), -1, nullptr, 0);
    if (wlen <= 0) {
        throw std::system_error(GetLastError(), std::system_category(), "MultiByteToWideChar failed");
    }
    
    // 转换为宽字符
    std::vector<wchar_t> wbuf(wlen);
    if (!MultiByteToWideChar(codePage, 0, gbk.c_str(), -1, wbuf.data(), wlen)) {
        throw std::system_error(GetLastError(), std::system_category(), "MultiByteToWideChar failed");
    }
    
    // 计算所需 UTF-8 缓冲区大小
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) {
        throw std::system_error(GetLastError(), std::system_category(), "WideCharToMultiByte failed");
    }
    
    // 转换为 UTF-8
    std::vector<char> ubuf(ulen);
    if (!WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, ubuf.data(), ulen, nullptr, nullptr)) {
        throw std::system_error(GetLastError(), std::system_category(), "WideCharToMultiByte failed");
    }
    
    return std::string(ubuf.data());
}

#else
#include <iconv.h>
#include <cstring>

// Unix-like 平台：使用 iconv 库进行 GBK -> UTF-8 转换
std::string Helper::gbk_to_utf8(const std::string &gbk)
{
    iconv_t cd = iconv_open("UTF-8//IGNORE", "GBK");
    if (cd == (iconv_t)-1)
        return "";

    size_t inlen = gbk.size();
    size_t outlen = inlen * 2 + 1;  // UTF-8 可能比 GBK 长
    std::string utf8(outlen, 0);

    char *inbuf = const_cast<char *>(gbk.data());
    char *outbuf = utf8.data();
    size_t inbytesleft = inlen;
    size_t outbytesleft = outlen;

    if (iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft) == (size_t)-1)
    {
        iconv_close(cd);
        return "";
    }

    iconv_close(cd);
    utf8.resize(outlen - outbytesleft);  // 去除未使用部分
    return utf8;
}
#endif

std::string Helper::utf8_to_gbk(const std::string &utf8Str)
{
    if (utf8Str.empty()) return "";
    
    // 获取本地代码页
    UINT codePage = CP_ACP;
    
    // 计算所需宽字符缓冲区大小
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (wlen <= 0) {
        throw std::system_error(GetLastError(), std::system_category(), "MultiByteToWideChar failed");
    }
    
    // 转换为宽字符
    std::vector<wchar_t> wbuf(wlen);
    if (!MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wbuf.data(), wlen)) {
        throw std::system_error(GetLastError(), std::system_category(), "MultiByteToWideChar failed");
    }
    
    // 计算所需本地编码缓冲区大小
    int llen = WideCharToMultiByte(codePage, 0, wbuf.data(), -1, nullptr, 0, nullptr, nullptr);
    if (llen <= 0) {
        throw std::system_error(GetLastError(), std::system_category(), "WideCharToMultiByte failed");
    }
    
    // 转换为本地编码
    std::vector<char> lbuf(llen);
    if (!WideCharToMultiByte(codePage, 0, wbuf.data(), -1, lbuf.data(), llen, nullptr, nullptr)) {
        throw std::system_error(GetLastError(), std::system_category(), "WideCharToMultiByte failed");
    }
    
    return std::string(lbuf.data());
}