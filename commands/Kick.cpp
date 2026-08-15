#include "Commands.hpp"

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

	Channel* chan = server.is_channel_exist(params[0]);
	if (!chan)
	{
		client.send_message("403 :No such channel", client.get_fd());
		return ;
	}

	if (!chan->is_member(client.get_fd()))
	{
		client.send_message("442 :You're not on that channel", client.get_fd());
		return ;
	}

	if (!chan->is_operator(client.get_fd()))
	{
		client.send_message("482 :You're not channel operator", client.get_fd());
		return ;
	}

	int target_fd = server.get_client_fd(params[1]);
	if (target_fd == -1)
	{
		client.send_message("401 :No such nick", client.get_fd());
		return ;
	}

	if (!chan->is_member(target_fd))
	{
		client.send_message("441 :They aren't on that channel", client.get_fd());
		return ;
	}

	std::string kickMsg = ":" + client.get_nickname() + " KICK " + chan->get_name()
						+ " " + params[1] + "\r\n";
	chan->broadcast_message(kickMsg, -1);

	chan->remove_member(target_fd);
}

