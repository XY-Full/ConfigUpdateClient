#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <functional>
#include <map>
#include "common/NetPack.h"

class MessageHandler {
private:
    std::map<int, std::function<void(const NetPack&)>> messageHandlers;

public:
    MessageHandler() {
        // Register default handlers
        registerHandler(0, [](const NetPack& pack) {
            std::cout << "Received unknown message type: " << pack.getType() << std::endl;
        });
    }

    void registerHandler(int messageType, std::function<void(const NetPack&)> handler) {
        messageHandlers[messageType] = handler;
    }

    void unregisterHandler(int messageType) {
        messageHandlers.erase(messageType);
    }

    bool handleMessage(const NetPack& message) {
        auto it = messageHandlers.find(message.getType());
        if (it != messageHandlers.end()) {
            it->second(message);
            return true;
        }
        return false;
    }
};

#endif // MESSAGE_HANDLER_H
