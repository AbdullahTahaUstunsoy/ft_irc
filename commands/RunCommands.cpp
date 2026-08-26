#include "Commands.hpp"

void Commands::run_command(Client& client, const std::string& command, const std::vector<std::string>& params, Server& server)
{
		std::cout << " run commands = here 2" << std::endl;
    if (command == "PASS")
        Commands::Pass(client, params, server);
    else if (command == "NICK")
        Commands::Nick(client, params, server);
    else if (command == "USER")
        Commands::User(client, params, server);
    else if (command == "PING")
        Commands::Ping(client, params, server);
    else if (command == "PRIVMSG")
        Commands::Privmsg(client, params, server);
    else if (command == "QUIT")
        Commands::Quit(client, params, server);
    else if (command == "JOIN")
        Commands::Join(client, params, server);
    else if (command == "INVITE")
    {
        Commands::Invite(client, params, server);
		std::cout << " run commands = here 1" << std::endl;
    }
    else if (command == "PART")
        Commands::Part(client, params, server);
    else if (command == "TOPIC")
        Commands::Topic(client, params, server);
    else if (command == "MODE")
        Commands::Mode(client, params, server);
}