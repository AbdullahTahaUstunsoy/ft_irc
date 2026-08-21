#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include "Client.hpp"
#include <vector>
#include <algorithm>

class Channel
{
    private:
        std::string topic;
        std::string name;
        std::string password;
        bool invite_only;
        int member_limit;
        bool topic_restrictions;
        bool key;
        std::map<int, Client*> _members;
        std::vector<int> operators;
        std::vector<int> invited;
    public:
        Channel(std::string name);
        ~Channel();
        int get_member_limit();
        std::string get_name();
        
        std::string get_password();
        bool get_invite();
        bool get_topic_rest();
        bool get_key();


        void set_topic(std::string topic);
        void set_invite(int status);
        void set_topic_rest(int status);
        void set_key(int status, std::string password);
        void set_member_limit(int limit);
        void set_password(std::string topic);

        void add_member(Client* client);
        bool is_member(int fd);
        void add_operator(int fd);
        void remove_operator(int fd);
        void broadcast_message(std::string message, int fd );
        void left_channel(int fd);
        void left_channel(int fd, std::string message);
        std::map<int, Client*> get_members();
        std::string get_topic();
        bool is_operator(int fd);
        void remove_member(int fd);
        void add_invite(int fd);
	    bool is_invited(int fd);
};

#endif