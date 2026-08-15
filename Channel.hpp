#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include "Client.hpp"
#include <vector>

class Channel
{
    private:
        std::string topic;
        std::string name;
        std::string password;
        //bool invite_only;
        int member_limit;
        std::map<int, Client*> _members;
        std::vector<int> operators;
        std::vector<int> invited;
    public:
        Channel(std::string name);
        ~Channel();
        int get_member_limit();
        std::string get_name();
        std::string get_password();
        void set_topic(std::string topic);
        void set_password(std::string topic);
        void add_member(Client* client);
        bool is_member(int fd);
        void add_operator(int fd);
        void broadcast_message(std::string message, int fd );
        std::string get_topic();
        bool is_operator(int fd);
        void remove_member(int fd);
        void add_invite(int fd);
	    bool is_invited(int fd);
};

#endif