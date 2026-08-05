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

std::string Channel::get_topic()
{
    return(topic);
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