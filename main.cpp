#include "Server.hpp"

void argControl(const char *argv1, const char *argv2)
{
    if(argv1[0] == '\0')
        throw std::runtime_error("invalid port number");
    for(int i = 0; argv1[i] != '\0'; i++)
    {
        if(!(argv1[i] > '0' && argv1[i] < '9'))
            throw std::runtime_error("invalid port number");
        if(i >= 5)
            throw std::runtime_error("port number out of bounds");
    }
    int test = std::atoi(argv1);
    if(!(test >= 1 && test <= 65535))
        throw std::runtime_error("port number out of bounds");

    if(argv2[0] == '\0')
        throw std::runtime_error("invalid password");
}




int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    //argümanların doğruluğunu kontrol edeceğim.
    try{
        argControl(argv[1], argv[2]);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    try {
        Server server(port, argv[2]);
        server.configureServerSocket();
        server.runServer();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}