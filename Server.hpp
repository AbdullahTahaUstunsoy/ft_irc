#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
    private:
        int _serverFd;
    public:
        int getServerFd() const;
        void setServerFd(int fd);
};

#endif