#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

//send(client_socket, "PASS 1234\r\n", 11, 0);

void check_msg(std::vector<std::string> msg, int client_socket)
{
    std::string message;
    std::string nick;
    std::string send_msg;
    size_t pos;
    
    message = msg.at(3);
    nick.append(msg.at(0), 1, msg.at(0).size() - 1);
    pos = message.find("!rps");
    if (pos != 0 && pos != std::string::npos)
    {
        send_msg = "PRIVMSG #test :Hi " + nick + " the usage of bot <!rps your_choice> example:!rps rock\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    message.erase(0, 4);
    if (message.find("rock") == std::string::npos && message.find("scissors") == std::string::npos && message.find("paper") == std::string::npos)
    {
        send_msg = "PRIVMSG #test :invalid choice\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    message.compare("rock");
    if (message.compare("rock"))
    {
        send_msg = "PRIVMSG #test :draw\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    else
    {
        send_msg = "PRIVMSG #test :...\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
}

std::vector<std::string> split_msg(std::string& message)
{
    std::vector<std::string> splited_msg;
    int i = -1;
    while(++i < 4)
    {
        std::string part;
        size_t pos = message.find(' ');
        if (i == 3)
        {
            std::cout << "asfalksfjlsadf" << std::endl;
            part.append(message ,1 ,message.size() - 1);
            message.erase(0, message.size() - 1);
            std::cout << "-----" << part << "-----" << std::endl;
            splited_msg.push_back(part);
            break ;
        }
        pos = message.find(' ');
        part.append(message, 0, pos);
        std::cout << "-----" << part << "-----" << std::endl;
        message.erase(0, pos + 1);
        splited_msg.push_back(part);
    }
    return (splited_msg);    
}

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
                if (message.find("PRIVMSG") != std::string::npos)
                {
                    splited_msg = split_msg(message);
                    for(int i = 0; i < 4; i++)
                        std::cout << splited_msg.at(i) << std::endl;
                    if (splited_msg.at(3).find("!rps") != std::string::npos)    
                        check_msg(splited_msg, client_socket);
                }
            }
        }
        else
            return 1;
    } 

    close(client_socket);
    return (0);
}