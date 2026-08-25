#include "Channel.hpp"

Channel::Channel(std::string name) : name(name)
{
	this->invite_only = 0;
	this->member_limit = -1;
	this->topic_restrictions = 0;
	this->key = 0;
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
    return(this->password);
}


void Channel::add_member(Client* client)
{
    _members[client->get_fd()] = client;
}

void Channel::add_operator(int fd)
{
    operators.push_back(fd);
}

void Channel::remove_operator(int fd)
{
    std::vector<int>::iterator it = std::find(operators.begin(), operators.end(), fd);
    if (it != operators.end())
        operators.erase(it);
}

void Channel::broadcast_message(std::string message, int fd)
{
    std::map<int, Client*>::iterator sender_it = _members.find(fd);
    if (sender_it == _members.end())
        return;

    std::string formatted_message = ":" + sender_it->second->get_nickname() + " PRIVMSG " + this->get_name() + " :" + message + "\r\n";

    std::map<int, Client*>::iterator it;
    for(it = _members.begin() ; it != _members.end(); it++)
    {
        if (it->first != fd) 
        {
            it->second->send_message(formatted_message, it->first);
        }
    }
}

bool Channel::is_member(int fd)
{
    std::map<int, Client*>::iterator it_mem = _members.find(fd);

    if (it_mem == _members.end())
        return (false);
    return(true);
}



bool Channel::is_operator(int fd)
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == fd)
			return true;
	}
	return false;
}


void Channel::remove_member(int fd)
{
	_members.erase(fd);

	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == fd)
		{
			operators.erase(operators.begin() + i);
			break;
		}
	}
}

void Channel::add_invite(int fd)
{
	if (!is_invited(fd))
		invited.push_back(fd);
}

bool Channel::is_invited(int fd)
{
	for (size_t i = 0; i < invited.size(); i++)
	{
		if (invited[i] == fd)
			return true;
	}
	return false;
}




void Channel::left_channel(int fd, std::string message)
{
    //broadcast message
	(void)message;
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

void Channel::set_invite(int status)
{
	this->invite_only = status;
}

void Channel::set_key(int status, std::string password)
{
	this->key = status;
	if (status)
		this->password = password;
}

void Channel::set_topic_rest(int status)
{
	this->topic_restrictions = status;
}

void Channel::set_member_limit(int limit)
{
	this->member_limit = limit;	
}

bool Channel::get_invite()
{
	return(this->invite_only);
}
bool Channel::get_topic_rest()
{
	return(this->topic_restrictions);
}
bool Channel::get_key()
{
	return (this->key);
}



