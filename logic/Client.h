#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>
#include <mutex>
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "MessageHandler.h"
#include "common/NetPack.h"
#include "common/Log.h"

class Client {
private:
    static Client* instance;
    static std::mutex mutex;

    std::shared_ptr<ConfigManager> configManager;
    std::shared_ptr<ConnectionManager> connectionManager;
    std::shared_ptr<MessageHandler> messageHandler;
    bool isRunning;
    bool isConnected;

    Client(const std::string& configPath) {
        configManager = std::make_shared<ConfigManager>(configPath);
        std::string serverAddress = configManager->getConfig("server_address");
        int serverPort = std::stoi(configManager->getConfig("server_port"));
        connectionManager = std::make_shared<ConnectionManager>(serverAddress, serverPort);
        messageHandler = std::make_shared<MessageHandler>();
        setupDefaultHandlers();
        isRunning = false;
        isConnected = false;
    }

    void setupDefaultHandlers() {
        messageHandler->registerHandler(1, [this](const NetPack& pack) {
            LOG_INFO("Received response: " << pack.getData());
        });
    }

    bool connectToServer() {
        if (isConnected) return true;
        if (!connectionManager->isConnectedToServer()) {
            LOG_ERROR("Failed to connect to server");
            return false;
        }
        isConnected = true;
        return true;
    }

public:
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    static Client* getInstance(const std::string& configPath) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            instance = new Client(configPath);
        }
        return instance;
    }

    bool start() {
        if (isRunning) return true;
        if (!connectToServer()) {
            return false;
        }
        isRunning = true;
        LOG_INFO("Client started successfully");
        return true;
    }

    void stop() {
        if (isRunning) {
            connectionManager->disconnect();
            isRunning = false;
            isConnected = false;
            LOG_INFO("Client stopped successfully");
        }
    }

    bool sendRequest(const NetPack& request) {
        if (!isRunning) {
            LOG_ERROR("Client is not running");
            return false;
        }
        if (!connectToServer()) {
            return false;
        }
        return connectionManager->sendRequest(request);
    }

    bool receiveResponse(NetPack& response) {
        if (!isRunning) {
            LOG_ERROR("Client is not running");
            return false;
        }
        return connectionManager->receiveResponse(response);
    }

    void handleMessage(const NetPack& message) {
        messageHandler->handleMessage(message);
    }

    bool isRunning() const {
        return isRunning;
    }

    bool reloadConfig() {
        return configManager->saveConfig();
    }
};

#endif // CLIENT_H
