#include "Client.hpp"
#include "Server.hpp"
#include <map>

Client :: Client(int fd) : _fd(fd)
{}

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