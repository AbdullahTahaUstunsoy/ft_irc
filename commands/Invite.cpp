#include "Commands.hpp"

void Commands::Invite(Client &client, const std::vector<std::string> &params, Server &server)
{
	std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

	if (!client.is_register())
	{
		std::cout << "here 1" << std::endl;
		client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered", client.get_fd());
		return ;
	}

	if (params.size() != 2)
	{
		std::cout << "here 2" << std::endl;
		client.send_message(":" + srv_name + " 461 " + nick + " INVITE :Not enough parameters", client.get_fd());
		return ;
	}

	int target_fd = server.get_client_fd(params[0]);
	if (target_fd == -1)
	{
		std::cout << "here 3" << std::endl;
		client.send_message(":" + srv_name + " 401 " + nick + " " + params[0] + " :No such nick", client.get_fd());
		return ;
	}

	Channel* chan = server.is_channel_exist(params[1]);
	if (!chan)
	{
		std::cout << "here 4" << std::endl;
		client.send_message(":" + srv_name + " 403 " + nick + " " + params[1] + " :No such channel", client.get_fd());
		return ;
	}

	if (!chan->is_member(client.get_fd()))
	{
		std::cout << "here 5" << std::endl;
		client.send_message(":" + srv_name + " 442 " + nick + " " + params[1] + " :You're not on that channel", client.get_fd());
		return ;
	}

	if (chan->is_member(target_fd))
	{
		std::cout << "here 5" << std::endl;
		client.send_message(":" + srv_name + " 443 " + nick + " " + params[0] + " " + params[1] + " :is already on channel", client.get_fd());
		return ;
	}

	std::cout << "here 6" << std::endl;
	chan->add_invite(target_fd);
	client.send_message(":" + srv_name + " 341 " + nick + " " + params[0] + " " + params[1], client.get_fd());
	std::string inviteMsg = ":" + nick + " INVITE " + params[0] + " :" + params[1];
	client.send_message(inviteMsg, target_fd);
}
