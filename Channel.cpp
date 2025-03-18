/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:46:07 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:44:25 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
    this->name = "";
    this->is_private = false;
    this->limit = 0;
    this->topic = "";
    this->password = "";
    
}
Channel::Channel(const std::string& which_name)
{
    this->name = which_name;
    this->is_private = false;
    this->limit = 0;
    this->topic = "";
    this->password = "";    
}

std::string Channel::get_name() const
{
    return (name);
}


std::vector<Client>& Channel::get_clients()
{
    return (join_clients);
}

void Channel::add_client(Client client, bool is_admin)
{
        if (is_admin == true)
            admins.push_back(client.get_nickname());
        else
            users.push_back(client.get_nickname());
        join_clients.push_back(client);
}

void Channel::remove_client(const Client& client)
{
    std::vector<Client>::iterator it = std::find(join_clients.begin(), join_clients.end(), client);
    if (it != join_clients.end())
    {
        join_clients.erase(it);
    }
    std::vector<std::string>::iterator it2 = std::find(admins.begin(), admins.end(), client.get_nickname());
    if (it2 != admins.end())
    {
        admins.erase(it2);
    }
    std::vector<std::string>::iterator it3 = std::find(users.begin(), users.end(), client.get_nickname());
    if (it3 != users.end())
    {
        users.erase(it3);
    }
}

bool Channel::has_client(const Client& client) const 
{
    for (size_t i = 0; i < join_clients.size(); i++)
    {
        if (join_clients[i].get_nickname() == client.get_nickname())
        {
            return true;
        }
    }
    return false;
}

void Channel::set_topic_name(std::string tnickname) 
{
    this->tnickname = tnickname;
}

std::vector<std::string> Channel::get_admins() const
{
    return (admins);
}

std::vector<std::string>& Channel::get_users()
{
    return (users);
}

bool Channel::add_invited(const std::string& nickname)
{
        invited.push_back(nickname);
        return true;

}

std::vector<std::string> Channel::get_invited() const
{
    return (invited);
}

bool Channel::has_client(std::string nickname) const
{
    for (size_t i = 0; i < join_clients.size(); ++i)
    {
        if (join_clients[i].get_nickname() == nickname)
        {
            return true;
        }
    }
    return false;
}

void Channel::set_private(bool is_private)
{
    this->is_private = is_private;
}

void Channel::set_limit(int limit)
{
    this->limit = limit;
}

void Channel::set_topic(std::string topic)
{
    this->topic = topic;
}

void Channel::set_password(std::string password)
{
    this->password = password;
}

bool Channel::get_private() const
{
    return (is_private);
}

size_t Channel::get_limit() const
{
    return (limit);
}

std::string Channel::get_password() const
{
    return (password);
}

bool Channel::check_invited(std::string nickname) const
{
    for (size_t i = 0; i < invited.size(); ++i)
    {
        if (strcmp(invited[i].c_str(), nickname.c_str()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Channel::is_admin(const Client& client) const
{
    for (size_t i = 0; i < admins.size(); ++i)
    {
        if (strcmp(admins[i].c_str(), client.get_nickname().c_str()) == 0)
        {
            return true;
        }
    }
    return false;
}

void Channel::remove_user(std::string nickname)
{
    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i] == nickname)
        {
            users.erase(users.begin() + i);
        }
    }
}

bool Channel::is_admin(std::string nickname) const
{
    for (size_t i = 0; i < admins.size(); ++i)
    {
        if (admins[i] == nickname)
        {
            return true;
        }
    }
    return false;
}

void Channel::remove_client(std::string nickname)
{
    for (size_t i = 0; i < join_clients.size(); ++i)
    {
        if (join_clients[i].get_nickname() == nickname)
        {
            join_clients.erase(join_clients.begin() + i);
        }
    }
    for (size_t i = 0; i < admins.size(); ++i)
    {
        if (admins[i] == nickname)
        {
            admins.erase(admins.begin() + i);
        }
    }
    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i] == nickname)
        {
            users.erase(users.begin() + i);
        }
    }
}

void Channel::send_toAll(const std::string& message, int exclude_fd)
{
    for (size_t i = 0; i < join_clients.size(); i++)
    {
        int client_fd = join_clients[i].get_client_fd();
        if (client_fd != exclude_fd)
        {
            ft_send(client_fd, message.c_str(), message.length(), 0);
        }
    }
}

std::string Channel::get_client_nickname() const
{
    std::string clients;
    for (size_t i = 0; i < join_clients.size(); ++i)
    {
        if (is_admin(join_clients[i]))
        {
            clients += "@" + join_clients[i].get_nickname() + " ";
        }
        else if (!is_admin(join_clients[i]))
        {
            clients += join_clients[i].get_nickname() + " ";
        }
    }
    return clients;
}

std::string Channel::get_topic() const
{
    return (topic);
}


void Channel::sendToAll(const std::string& message) 
{
    for (size_t i = 0; i < join_clients.size(); i++)
    {
        int client_fd = join_clients[i].get_client_fd();
        ft_send(client_fd, message.c_str(), message.length(), 0);
    }
}

int Channel::check_join(Channel channel, int client_fd, Client client, std::string password)
{
    std::string reply = "";
    if(channel.get_private() == true && channel.check_invited(client.get_nickname()) == false)
        return (send_message(client_fd, ":" + client.get_nickname() + " " + std::string(ERR_INVITEONLYCHAN) + " " + channel.get_name() + " :Cannot join channel (+i)\r\n", 1));

    else if (channel.get_limit() != 0 && channel.get_clients().size() >= channel.get_limit())
        return (send_message(client_fd, ":" + client.get_nickname() + " " + std::string(ERR_CHANNELISFULL) + " " + channel.get_name() + " :Cannot join channel (+l)\r\n", 1));

    else if (channel.has_client(client) == true)
        return (send_message(client_fd, ":" + client.get_nickname() + " " + std::string(ERR_USERONCHANNEL) + " " + channel.get_name() + " :already in channel\r\n", 1));

    else if (channel.get_password().size() != 0)
    {
        if (!password.empty() && channel.get_password() != password)
            return (send_message(client_fd,  ":" + client.get_nickname() + " " + std::string(ERR_BADCHANNELKEY) + " " + channel.get_name() + " :Cannot join channel (+k) - bad key\r\n", 1));
        
        else if (password.empty())
            return (send_message(client_fd, ":" + client.get_nickname() + " " + std::string(ERR_BADCHANNELKEY) + " " + channel.get_name() + " :Cannot join channel (+k)\r\n", 1));
    }
    return 0;
}

int Channel::kick_check(Channel channel, int client_fd, Client client, std::string user, bool i)
{
    if (i == false)
       return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NOSUCHNICK) + " " + client.get_nickname() + " " + user + " :No such nick/channel\r\n", 1));

    if(!channel.has_client(client))
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NOTONCHANNEL) + " " + client.get_nickname() + " " + channel.get_name() + " :You're not on that channel\r\n", 1));
    
    else if (!channel.is_admin(client))
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel.get_name() + " :You're not channel operator\r\n", 1));

    else if (!channel.has_client(user))
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NOSUCHNICK) + " " + client.get_nickname() + " " + user + " :they are not on that channel\r\n", 1));
    return (0);
}

bool Channel::has_admins() const
{
    if (admins.size() > 0)
        return true;
    return false;
}