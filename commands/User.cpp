#include "Commands.hpp"

void Commands::User(Client& client, const std::vector<std::string>& params)
{
    if (params.size() != 4)
    {
        client.send_message("should be 4 parameter", client.get_fd());
        return ;
    }
    if (client.is_register())
    {
        client.send_message("user was already register", client.get_fd());
        return ;
    }
    client.set_user_name(params[0]);
    if (client.get_pass() == true && !(client.get_nickname().empty()))
    {
        client.set_real_name(params[3]);
        client.set_register();
    }
    else
    {
        client.send_message("pass nick is require", client.get_fd());
        std::cout << client.get_pass() << std::endl;
        std::cout << (client.get_nickname().empty()) << std::endl;
    }
    std::string welcome_msg = ":localhost 001 " + client.get_nickname() + " :Welcome to the Internet Relay Network " + client.get_nickname() + "\r\n";
    client.send_message(welcome_msg, client.get_fd());
}
