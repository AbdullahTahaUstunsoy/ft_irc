#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    send(client_socket, "PASS 1234\r\n", 11, 0);
    send(client_socket, "NICK bot\r\n", 10, 0);
    send(client_socket, "USER bot 0 * :Bot\r\n", 19, 0);
    send(client_socket, "JOIN #test\r\n", 12, 0);

    std::string buffer;

    while (true)
    {
        char temp_buf[512];

        ssize_t len = recv(client_socket, temp_buf, sizeof(temp_buf), 0);

        if (len > 0)
        {
            buffer.append(temp_buf, len);

            size_t pos = buffer.find("\r\n");

            if (pos != std::string::npos)
            {
                std::string message = buffer.substr(0, pos);
                buffer.erase(0, pos + 2);
                std::cout << "Message received:\n" << message << std::endl;
                std::vector<std::string> splited_msg;
                for (size_t i = 0; i < 4 ; i++)
                {
                    size_t pos = message.find(' ');
                    if (pos == std::string::npos)
                        pos = message.size();
                    if (i == 3 )
                        pos = message.size();
                    std::string part;
                    part.append(message, 0, pos);
                    std::cout << "-----" << part << "-----" << std::endl;
                    message.erase(0, pos + 1);
                    splited_msg.push_back(part);
                }
                
            }
        }
        else
            return 1;
    } 

    close(client_socket);
    return (0);
}