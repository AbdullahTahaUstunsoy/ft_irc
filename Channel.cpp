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

void Channel::add_member(Client* client)
{
    _members[client->get_fd()] = client;
}