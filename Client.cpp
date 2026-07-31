#include "Client.hpp"
#include "Server.hpp"

Client :: Client(int fd) : _fd(fd), registered(false), pass(false)
{
}

bool Client::is_register()
{
	return (registered);
}

void Client::set_pass()
{
	pass = true;
}
bool Client::get_pass()
{
	return(pass);
}

std::string Client::get_nickname()
{	
	return (nickname);
}

void Client::set_register()
{
	registered = true;
}

void Client::send_message(std::string msg,int client_fd)
{
	send(client_fd, msg.c_str(), msg.length(), 0);
	return ;
}

void Client::set_nick_name(std::string nick)
{
	this->nickname = nick;
}

int Client ::  get_fd()
{
	return (this->_fd);
}

void Client :: add_buffer(const char *data, size_t len)
{
	this->buffer.append(data, len);	
}

bool Client :: line_end_check(std::string &line)
{
	size_t pos;

	pos = this->buffer.find('\n');
	if (pos == std::string::npos)
		return (false);

	line = this->buffer.substr(0, pos + 1);
	this->buffer.erase(0, pos + 1);
	return (true);
}

std::string Client :: get_buffer()
{
	return (this->buffer);
}


Client :: ~Client()
{}