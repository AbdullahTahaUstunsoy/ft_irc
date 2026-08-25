#include "Commands.hpp"

void Commands::Invite(Client &client, const std::vector<std::string> &params, Server &server)
{
	if (!client.is_register())
	{
		std::cout << "here 1" << std::endl;
		client.send_message("should register", client.get_fd());
		return ;
	}

	if (params.size() != 2)
	{
		std::cout << "here 2" << std::endl;
		client.send_message("461 INVITE :Not enough parameters", client.get_fd());
		return ;
	}

	int target_fd = server.get_client_fd(params[0]);
	if (target_fd == -1)
	{
		std::cout << "here 3" << std::endl;
		client.send_message("401 :No such nick", client.get_fd());
		return ;
	}

	Channel* chan = server.is_channel_exist(params[1]);
	if (!chan)
	{
		std::cout << "here 4" << std::endl;
		client.send_message("403 :No such channel", client.get_fd());
		return ;
	}

	if (!chan->is_member(client.get_fd()))
	{
		std::cout << "here 5" << std::endl;
		client.send_message("442 :You're not on that channel", client.get_fd());
		return ;
	}

	if (chan->is_member(target_fd))
	{
		std::cout << "here 5" << std::endl;
		client.send_message("443 :User is already on that channel", client.get_fd());
		return ;
	}

	std::cout << "here 6" << std::endl;
	chan->add_invite(target_fd);
	client.send_message("341 " + params[0] + " " + params[1] + " :Inviting", client.get_fd());
	std::string inviteMsg = ":" + client.get_nickname() + " INVITE " + params[0] + " " + params[1] + "\r\n";
	client.send_message(inviteMsg, target_fd);
}