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
    public:
        Channel(std::string name);
        ~Channel();
        int get_member_limit();
        std::string get_name();
        std::string get_password();
        void set_topic(std::string topic);
        void set_password(std::string topic);
        void add_member(Client* client);
        void add_operator(int fd);
        bool is_mem(int fd);
        bool is_op(int fd);
        void broadcast_message(std::string& message, int fd );
        void left_channel(int fd);
        void left_channel(int fd, std::string message);
        std::map<int, Client*> get_members();
        std::string get_topic();
};

#endif