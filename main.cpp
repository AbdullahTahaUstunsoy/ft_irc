#include <iostream>
#include <string>
#include "inc/Client.hpp"
#include "inc/Channel.hpp"
#include "inc/Parser.hpp"
#include "inc/Server.hpp"



void feedChunk(Client &client, const std::string &chunk)
{
	std::cout << ">> [fd " << client.get_fd() << "] recv() chunk: \""
			  << chunk << "\"" << std::endl;

	client.add_buffer(chunk.c_str(), chunk.size());

	std::string line;
	while (client.line_end_check(line))
	{
		std::cout << "   -> complete line extracted: \"" << line << "\"" << std::endl;

		Parser parser(line);
		if (parser.parse_message() == 0)
			std::cout << "Done succesfly " << std::endl;
	}
}

int main()
{
	Client client1(4);
	Client client2(5);

	std::cout << "=== Client 1: message split across two recv() calls ===" << std::endl;
	feedChunk(client1, "NICK john");
	feedChunk(client1, "doe\r\n");

	std::cout << std::endl << "=== Client 1: two full messages in one recv() ===" << std::endl;
	feedChunk(client1, "USER johndoe 0 * :John Doe\r\nJOIN #general\r\n");

	std::cout << std::endl << "=== Client 2: message split mid-way + second message ===" << std::endl;
	feedChunk(client2, "PRIVMSG #general :Hello everyone, how are");
	feedChunk(client2, " you doing?\r\nPING :server1\r\n");

	std::cout << std::endl << "=== Client 2: three messages in one recv() ===" << std::endl;
	feedChunk(client2, "NICK alice\r\nUSER alice 0 * :Alice A\r\nQUIT :bye\r\n");

	std::cout << std::endl << "=== Client 1: lowercase + invalid command ===" << std::endl;
	feedChunk(client1, "join lowercase\r\nFOO bar baz\r\n");

	std::cout << std::endl << "=== Client 1: prefixed message ===" << std::endl;
	feedChunk(client1, ":johndoe!john@localhost PRIVMSG #general :Hey there!\r\n");

	std::cout << std::endl << "=== Client 2: partial data, no \\n yet ===" << std::endl;
	feedChunk(client2, "KICK #general baduser");
	std::cout << "   (correctly nothing parsed -- waiting for more data)" << std::endl;

	std::cout << std::endl << "leftover in client2 buffer: \""
			  << client2.get_buffer() << "\"" << std::endl;

	return (0);
}
