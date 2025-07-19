#pragma once
#ifdef _WIN32
#else
#include <vector>

class EpollWrapper 
{
public:
    EpollWrapper();
    ~EpollWrapper();

    bool add(int fd);
    bool remove(int fd);
    std::vector<int> wait(int timeout_ms = 1000);

private:
    int epoll_fd_;
};
#endif