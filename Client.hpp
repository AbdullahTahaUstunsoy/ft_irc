#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		int _fd;
		std::string buffer;
		std::string _sendBuffer;
	public:
		Client(int fd);
		int get_fd();
		void add_buffer(const char *data, size_t len);
		bool line_end_check(std::string &line);
		std::string get_buffer();
		~Client();
};

#endif