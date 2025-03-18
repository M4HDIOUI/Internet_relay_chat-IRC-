/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:47:29 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:41:38 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include "utils.hpp"

int Server::kick_cmd(std::string buffer, int client_fd)
{
    Client client = clients[client_fd];
    
    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);
    
    std::string user = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, user.length() + 1);
    
    std::string reason = buffer.substr(0, buffer.length());
    if (reason[0] == ':')
        reason.erase(0, 1);
    if (reason.empty())
        reason = user;
    
    if (user.empty() || channel_name.empty())
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_usrname() + " :Not enough parameters\r\n", 1));
    if (check_channel_if_exists(channel_name) == false)
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_usrname() + " " + channel_name + " :No such channel\r\n", 1));
    for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        
        if (it->get_name() == channel_name)
        {
            if(it->kick_check(*it, client_fd, client, user, check_client_if_exists(user)))
                return (1);
            else
            {
                std::string reply = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " KICK " + channel_name + " " + user + " :" + reason + "\r\n";
                ft_send(client_fd, reply.c_str(), reply.length(), 0);
                it->send_toAll(reply, client_fd);
                it->remove_client(user);
                if (it->get_clients().size() == 0)
                {
                    channels.erase(it);
                    return (0);
                }
            }
        }
    }
    return(0);
}