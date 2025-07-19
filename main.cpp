#include "TcpClient.h"
#include "NetPack.h"
#include "Timer.h"
#include <memory>
#include <string>

Channel < std::pair<int64_t, std::shared_ptr<NetPack>> > in_channel_;
Channel < std::pair<int64_t, std::shared_ptr<NetPack>> > out_channel_;
Timer timer_;

int32_t main()
{
    const std::string addr = "192.168.5.123";
    TcpClient client(addr, 8887, &in_channel_, &out_channel_, &timer_);
    client.start();
}