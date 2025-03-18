/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:47:07 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 04:47:08 by omahdiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include <unistd.h>

int Server::get_fd_by_nickname(std::string nickname)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (it->second.get_nickname() == nickname)
            return (it->first);
    }
    return (-1);
}

int Server::invite_cmd(std::string buffer, int socker_fd)
{
    Client client = clients[socker_fd];
    
    std::string user = buffer.substr(0,buffer.find(" "));
    buffer.erase(0, user.length() +1);
    
    std::string channel_name  = buffer.substr(0,buffer.find(" "));
    
    if (user.empty() || channel_name.empty())
        return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_usrname() + " :Not enough parameters\r\n", 1));
        
    if (check_channel_if_exists(channel_name) == false)
        return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_usrname() + " " + channel_name + " :No such channel\r\n", 1));

    for (std::vector<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
    {
        if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (!it->has_client(client) || !it->is_admin(client))
                return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_NOTONCHANNEL) + " " + client.get_usrname() + " " + channel_name + " :You're not on that channel\r\n", 1));

            if (it->has_client(user))
                return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_USERONCHANNEL) + " " + client.get_usrname() + " " + channel_name + " :User is already on channel\r\n", 1));

            if (check_client_if_exists(user) == false)
                return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_NOSUCHNICK) + " " + client.get_usrname() + " " + user + " :No such nick/channel\r\n", 1));
            else 
            {
                it->add_invited(user);
                std::string message = ":IRC-localhost " + std::string(RPL_INVITING) + " " + client.get_nickname() + " " + user + " " + channel_name + "\r\n";
                ft_send(socker_fd, message.c_str(), message.length(), 0);
                return (send_message(get_fd_by_nickname(user), ":" + client.get_nickname() + "!~" + client.get_usrname() + client.get_ip() + " INVITE " + user + " :" + channel_name + "\r\n", 0));
            }
            break;
        }
    }
    return (0);
}