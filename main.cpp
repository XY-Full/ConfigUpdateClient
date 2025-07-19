#include "TcpClient.h"
#include "NetPack.h"
#include "Timer.h"

Channel<int64_t, std::shared_ptr<NetPack>> in_channel_;
Channel<int64_t, std::shared_ptr<NetPack>> out_channel_;
Timer timer_;

int32_t main()
{
    TcpClient client("192.168.5.123", 8887, in_channel_, out_channel_, timer_);
    client.start();

    
}