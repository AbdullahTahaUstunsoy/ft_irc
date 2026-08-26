#include "Commands.hpp"

void Commands::Kick(Client &client, const std::vector<std::string> &params, Server &server)
{
	std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

	if (!client.is_register())
	{
		client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered", client.get_fd());
		return ;
	}

	if (params.size() < 2)
	{
		client.send_message(":" + srv_name + " 461 " + nick + " KICK :Not enough parameters", client.get_fd());
		return ;
	}

	Channel* chan = server.is_channel_exist(params[0]);
	if (!chan)
	{
		client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
		return ;
	}

	if (!chan->is_member(client.get_fd()))
	{
		client.send_message(":" + srv_name + " 442 " + nick + " " + params[0] + " :You're not on that channel", client.get_fd());
		return ;
	}

	if (!chan->is_operator(client.get_fd()))
	{
		client.send_message(":" + srv_name + " 482 " + nick + " " + params[0] + " :You're not channel operator", client.get_fd());
		return ;
	}

	int target_fd = server.get_client_fd(params[1]);
	if (target_fd == -1)
	{
		client.send_message(":" + srv_name + " 401 " + nick + " " + params[1] + " :No such nick", client.get_fd());
		return ;
	}

	if (!chan->is_member(target_fd))
	{
		client.send_message(":" + srv_name + " 441 " + nick + " " + params[1] + " " + params[0] + " :They aren't on that channel", client.get_fd());
		return ;
	}

	std::string kickMsg = ":" + nick + " KICK " + chan->get_name() + " " + params[1] + + "\r\n";
	chan->broadcast_message(kickMsg, -1);
	chan->remove_member(target_fd);
}

