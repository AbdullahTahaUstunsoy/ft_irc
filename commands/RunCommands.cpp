#include "Commands.hpp"

void Commands::run_command(Client& client, const std::string& command, const std::vector<std::string>& params, Server& server)
{
    if (command == "PASS")
        Commands::Pass(client, params, server);
    else if (command == "NICK")
        Commands::Nick(client, params, server);
    else if (command == "USER")
        Commands::User(client, params);
    else if (command == "PING")
        Commands::Ping(client, params);
    else if (command == "PRIVMSG")
        Commands::Privmsg(client, params, server);
    else if (command == "QUIT")
        Commands::Quit(client, params, server);
    else if (command == "JOIN")
        Commands::Join(client, params, server);
}