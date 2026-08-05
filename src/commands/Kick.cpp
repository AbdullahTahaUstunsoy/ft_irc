#include "../../inc/Commands.hpp"

void Commands::Kick(Client &client, const std::vector<std::string> &params, Server &server)
{
	if (!client.is_register())
    {
        client.send_message("should register", client.get_fd());
        return ;
    }

	if (params.size() < 2)
	{
		client.send_message("KICK :Not enough parameters", client.get_fd());
		return ;
	}

	std::string channel_name = params[0];
	std::string user_to_kick = params[1];
	if (params.size() >= 3)
		std::string reason = param[2];
	
	//Channel gelince devam edilecek
	
	
}