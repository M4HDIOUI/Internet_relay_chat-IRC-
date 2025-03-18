/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:50:18 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:47:44 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"

void Server::get_topic(std::string channel_name, int client_fd)
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (std::strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            std::string message = ":" + it->get_topic() + "\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
            return;
        }
    }
}

void Server::unset_topic(std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (it->is_admin(client.get_nickname()) == false)
            {
                std::string reply = ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " :You're not channel operator\r\n";
                ft_send(client_fd, reply.c_str(), reply.length(), 0);
                return;
            }
            it->set_topic("");
            std::string message = ":" + it->get_name() + " TOPIC :" + it->get_topic() + "\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
            return;
        }
    }
}

void Server::set_topic(std::string channel_name, std::string buffer, int client_fd)
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            it->set_topic(buffer);
            std::string message = ":" + it->get_name() + " TOPIC :" + it->get_topic() + "\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
            return;
        }
    }
}

int Server::topic_cmd(std::string buffer, int client_fd)
{
    Client client = clients[client_fd];
    Channel channel;
    if(buffer.empty())
    {
        std::string reply = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " :Not enough parameters\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
        return (1);
    }
    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);
    if (check_channel_if_exists(channel_name) == false)
    {
        std::string reply = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
        return (1);
    }
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->has_client(client) == false)
        {
            std::string reply = ":IRC-localhost " + std::string(ERR_NOTONCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :You're not on that channel\r\n";
            ft_send(client_fd, reply.c_str(), reply.length(), 0);
            return (1);
        }
        if (it->is_admin(client) == false)
        {
            std::string reply = ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " :You're not a channel operatorr\n";
            ft_send(client_fd, reply.c_str(), reply.length(), 0);
            return (1);
        }
    }
    if (buffer.empty())
        get_topic(channel_name, client_fd);
    else if (buffer == ":")
        unset_topic(channel_name, client_fd);
    else
    {
        if (!buffer.empty() && buffer[0] == ':')
            buffer.erase(0, 1);
        set_topic(channel_name, buffer, client_fd);
    }
    channel.set_topic_name(client.get_nickname());
    return (0);
}