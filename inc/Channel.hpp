#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include "Client.hpp"

class Channel
{
    private:
        std::string topic;
        std::string name;
        std::string password;
        bool invite_only;
        int member_limit;
        std::map<int, Client*> _members;
    public:
        Channel(std::string name);
        ~Channel();
        int get_member_limit();
        std::string get_name();
        void add_member(Client* client);
        std::string get_topic();
};

#endif