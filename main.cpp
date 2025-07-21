#include "ConfigClient.h"
#include "NetPack.h"
#include "Timer.h"
#include <memory>
#include <string>
#include <windows.h>


int32_t main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    const std::string addr = "192.168.5.123";
    ConfigClient client(addr, 8887);
    client.start();
    client.join();
}