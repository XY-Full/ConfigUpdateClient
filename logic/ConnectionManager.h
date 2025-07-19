#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <string>
#include <memory>
#include "common/SocketWrapper.h"
#include "common/NetPack.h"

class ConnectionManager {
private:
    std::shared_ptr<SocketWrapper> socket;
    std::string serverAddress;
    int serverPort;
    bool isConnected;

    bool connectToServer() {
        if (isConnected) return true;
        socket = std::make_shared<SocketWrapper>();
        if (!socket->create()) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        if (!socket->connect(serverAddress, serverPort)) {
            std::cerr << "Failed to connect to server" << std::endl;
            return false;
        }
        isConnected = true;
        return true;
    }

public:
    ConnectionManager(const std::string& address, int port)
        : serverAddress(address), serverPort(port), isConnected(false) {}

    bool sendRequest(const NetPack& request) {
        if (!isConnected && !connectToServer()) {
            return false;
        }
        return socket->send(request);
    }

    bool receiveResponse(NetPack& response) {
        if (!isConnected && !connectToServer()) {
            return false;
        }
        return socket->receive(response);
    }

    void disconnect() {
        if (socket) {
            socket->close();
            isConnected = false;
        }
    }

    bool isConnectedToServer() const {
        return isConnected;
    }
};

#endif // CONNECTION_MANAGER_H
