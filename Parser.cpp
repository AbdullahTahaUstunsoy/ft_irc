#include "Parser.hpp"

Parser :: Parser() {}

void Parser :: get_message(std::string raw_message)
{
    this->message = raw_message;
}

void to_upper(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
}

int Parser :: parse_message()
{
	std::string parse_msg = this->message;
	std::string erase_chars = "\r\n ";
	size_t pos;
	size_t last = message.find_last_not_of(erase_chars);

	std::cout << "-------------------------" << std::endl;
	std::cout << "Starting Message: "<<parse_msg << std::endl;
	if (last != std::string::npos)
    	parse_msg.erase(last + 1);
	else
    	parse_msg.clear();
	
	if (parse_msg[0] == ':')
	{
		pos = parse_msg.find(' ');
		if (pos != std::string::npos)
		{
			std::string word = parse_msg.substr(0, pos);
			parse_msg.erase(0, pos + 1);
		}
	}

	pos = parse_msg.find(' ');
	if (pos != std::string::npos)
	{
		std::string cmd = parse_msg.substr(0, pos);
		to_upper(cmd);
		if (is_valid_command(cmd))
		{
			this->command = cmd;
			parse_msg.erase(0, pos + 1);
			std::cout << "Command: "<< this->command << std::endl;
		}
		else
		{
			std::cout << "NO COMMAND (HATA MESAJI DÜZELTİLECEK) " << std::endl;
			return (1);
		}
	}
	while (!parse_msg.empty())
	{
		if (parse_msg[0] == ':')
		{
			this->parameters.push_back(parse_msg.substr(1));   // drop the ':' itself, take the rest
			break;
		}
		pos = parse_msg.find(' ');
		if (pos == std::string::npos)
		{
			this->parameters.push_back(parse_msg);
			break;
		}
		else
		{
			this->parameters.push_back(parse_msg.substr(0, pos));
			parse_msg.erase(0, pos + 1);
		}
	}
	std::cout << "---- parameters ----" << std::endl;
	for (size_t i = 0; i < this->parameters.size(); i++)
		std::cout << "param[" << i << "]: \"" << this->parameters[i] << "\"" << std::endl;
	std::cout << "---------------------" << std::endl;
	
	return (0);
}

bool Parser :: is_valid_command(const std::string &cmd) const
{
	static const std::string commands[14] = {
		"PASS", "NICK", "USER", "JOIN", "PRIVMSG",
		"PART", "KICK", "INVITE", "TOPIC", "MODE",
		"PING", "PONG", "QUIT", "CAP"
	};
	for (int i = 0; i < 14; i++)
	{
		if (cmd == commands[i])
			return (true);
	}
	return (false);
}

const std::string &Parser :: get_command() const
{
	return (this->command);
}

const std::vector<std::string>& Parser:: get_parameters() const
{
	return (this->parameters);
}

void Parser :: clear_params()
{
	this->parameters.clear();
}

Parser:: ~Parser()
{
	
}