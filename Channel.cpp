#include "Channel.hpp"

Channel::Channel(std::string name) : name(name)
{

}

Channel::~Channel()
{

}

int Channel::get_member_limit()
{
    return(this->member_limit);
}

std::string Channel::get_name()
{
    return(this->name);
}

std::string Channel::get_topic()
{
    return (this->topic);
}

void Channel::set_topic(std::string topic)
{
    this->topic = topic;
}

void Channel::set_password(std::string password)
{
    this->password = password;
}


std::string Channel::get_password()
{
    return(password);
}

void Channel::add_member(Client* client)
{
    _members[client->get_fd()] = client;
}

void Channel::add_operator(int fd)
{
    operators.push_back(fd);
}

void Channel::broadcast_message(std::string& message, int fd)
{
    
}

bool Channel::is_mem(int fd)
{
    if (_members.find(fd) != _members.end())
        return (true);
    return (false);
}

void Channel::left_channel(int fd, std::string message)
{
    //broadcast message
    _members.erase(fd);
}

void Channel::left_channel(int fd)
{
    //broadcast message
    _members.erase(fd);
}

std::map<int, Client*> Channel::get_members()
{
    return (_members);
}

bool Channel::is_op(int fd)
{
    for (int i = 0; i < operators.size(); i++)
    {
        if (operators[i] == fd)
            return (true);
    }
    return (false);
}