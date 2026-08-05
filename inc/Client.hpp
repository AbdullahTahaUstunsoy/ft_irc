#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>

class Client
{
	private:
		int _fd;
		std::string nickname;
    	std::string username;
		std::string realname;
		std::string buffer;
		bool registered;
		bool pass;
	public:
		Client(int fd);
		int get_fd();
		void add_buffer(const char *data, size_t len);
		bool line_end_check(std::string &line);
		std::string get_buffer();
		std::string get_nickname();
		bool get_pass();
		bool is_register();
		void set_pass();
		void set_register();
		void set_nick_name(std::string nick);
		void set_user_name(std::string user);
		void set_real_name(std::string realname);
		void send_message(std::string msg, int client_fd);
		~Client();
};

#endif