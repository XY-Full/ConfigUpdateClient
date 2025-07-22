#include "ConfigClient.h"
#include "NetPack.h"
#include "Timer.h"
#include <memory>
#include <string>
#include <windows.h>


int32_t main()
{
    // SetConsoleCP(CP_UTF8);
    // SetConsoleOutputCP(CP_UTF8);

    // 设置控制台字体以支持中文
    // CONSOLE_FONT_INFOEX fontInfo = { sizeof(fontInfo) };
    // GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    // wcscpy_s(fontInfo.FaceName, L"Consolas");
    // SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);

    const std::string addr = "192.168.5.123";
    ConfigClient client(addr, 8887);
    client.start();
    client.join();
}