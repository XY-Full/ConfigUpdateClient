#pragma once
#include "TcpClient.h"
#include "NetPack.h"
#include "Timer.h"
#include "Channel.h"
#include <thread>
#include <atomic>
#include <memory>
#include <utility>
#include "JsonConfig.h"
#include "JsonConfigNode.h"

class JsonConfig;

class ConfigClient 
{
public:
    ConfigClient(const std::string& host, uint16_t port);
    ~ConfigClient();

    void start();
    void stop();
    void join();

private:
    int32_t inputLoop();
    void receiveLoop();
    void heartLoop();

    void handleUserChoice(int choice);
    void handleServerResponse(const std::shared_ptr<NetPack>& pack);

private:
    std::string host_;
    uint16_t port_;
    std::unique_ptr<TcpClient> tcpClient_;
    std::atomic<bool> running_;

    Channel<std::pair<int64_t, std::shared_ptr<NetPack>>> in_channel_;
    Channel<std::pair<int64_t, std::shared_ptr<NetPack>>> out_channel_;
    Timer timer_;

    std::thread input_thread_;
    std::thread receive_thread_;
    JsonConfig configMaker_;
};
