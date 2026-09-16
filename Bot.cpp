#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

int result(std::string& choice)
{
    srand(time(NULL));
    int num = rand(); 

    // 0 rock 1 paper 2 scissors
    // 0 draw 1 won 2 lost
    if (choice == "rock")
    {
        if (num % 3 == 0)
            return (0);
        else if (num % 3 == 1)
            return (2);
        else
            return (1);
    }
    else if (choice == "paper")
    {
        if (num % 3 == 0)
            return (1);
        else if (num % 3 == 1)
            return (0);
        else
            return (2);
    }
    else
    {
        if (num % 3 == 0)
            return (2);
        else if (num % 3 == 1)
            return (1);
        else
            return (0);
    }
}

void check_msg(std::vector<std::string> msg, int client_socket)
{
    std::string message;
    std::string nick;
    std::string send_msg;
    size_t pos;
    std::string choice;
    std::string channel_name;
    int val;
    
    message = msg.at(3);
    channel_name = msg.at(2);
    nick.append(msg.at(0), 1, msg.at(0).size() - 1);
    pos = message.find("!rps");
    int i = -1;
    while (isspace(message[++i]))
        ;
    if (i != pos)
    {
        send_msg = "PRIVMSG " + channel_name + " :Hi " + nick + " the usage of bot <!rps your_choice(rock,paper,scissors)> example:!rps rock\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    i += 4;
    while (isspace(message[++i]))
        ;
    message.erase(0, i);
    if (message != "rock" && message != "scissors" && message != "paper")
    {
        send_msg = "PRIVMSG " + channel_name + " :Hi " + nick + " the usage of bot <!rps your_choice(rock,paper,scissors)> example:!rps rock\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    if (message.compare("rock") == 0)
        choice = "rock";
    else if (message.compare("paper") == 0)
        choice = "paper";
    else
        choice = "scissors";
    val = result(choice);
    if (val == 0)
    {
        send_msg = "PRIVMSG " + channel_name + " :" + nick + " you drew the match\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    else if (val == 1)
    {
        send_msg = "PRIVMSG " + channel_name + " :" + nick + " you won\r\n";
        send(client_socket, send_msg.c_str(), send_msg.size(),0);
        return ;
    }
    else
    {
        send_msg = "PRIVMSG " + channel_name + " :" + nick + " you lost\r\n";
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
    sockaddr_in server_address;
    std::string buffer;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (argc != 2)
    {
        std::cout << "invalid usage the usage is <./a.out channel_name>" << std::endl;
        return (1);
    }
    std::string channel_name = argv[1];
    channel_name = '#' + channel_name;    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = INADDR_ANY;
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    send(client_socket, "PASS 1234\r\n", 11, 0);
    send(client_socket, "NICK bot\r\n", 10, 0);
    send(client_socket, "USER bot 0 * :Bot\r\n", 19, 0);
    std::string join_msg = "JOIN " + channel_name + "\r\n";
    send(client_socket, join_msg.c_str(), join_msg.size(), 0);
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
                std::vector<std::string> splited_msg;
                if (message.find("PRIVMSG") != std::string::npos)
                {
                    splited_msg = split_msg(message);
                    if (splited_msg.at(3).find("!rps") != std::string::npos)    
                        check_msg(splited_msg, client_socket);
                }
            }
        }
        else
            return (1);
    } 
    close(client_socket);
    return (0);
}