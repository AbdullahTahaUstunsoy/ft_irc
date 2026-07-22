#include <iostream>
#include <string>
#include "inc/Client.hpp"
#include "inc/Channel.hpp"
#include "inc/Parser.hpp"
#include "inc/Server.hpp"

int main()
{
std::string irc_messages[10] = {
	"PASS mypassword123\r\n",
	"NICK johndoe\r\n",
	"USER johndoe 0 * :John Doe\r\n",
	"JOIN #general\r\n",
	"PRIVMSG #general :Hello everyone, how are you doing?\r\n",
	":johndoe!john@localhost PRIVMSG #general :Hey there!\r\n",
	"QUIT :Leaving now\r\n",
	"PING :server1\r\n",
	"join lowercase\r\n",
	"FOO bar baz\r\n"
};

	Parser parse(irc_messages[0]);
	Parser parse1(irc_messages[1]);
	Parser parse2(irc_messages[2]);
	Parser parse3(irc_messages[3]);
	Parser parse4(irc_messages[4]);
	Parser parse5(irc_messages[5]);
	Parser parse6(irc_messages[6]);
	Parser parse7(irc_messages[7]);
	Parser parse8(irc_messages[8]);
	Parser parse9(irc_messages[9]);
	parse.parse_message();
	parse1.parse_message();
	parse2.parse_message();
	parse3.parse_message();
	parse4.parse_message();
	parse5.parse_message();
	parse6.parse_message();
	parse7.parse_message();
	parse8.parse_message();
	parse9.parse_message();

	return (0);
}
