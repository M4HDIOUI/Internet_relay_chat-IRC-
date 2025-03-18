/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:48:53 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:46:57 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include "client.hpp"

int Server::part_cmd(std::string buffer, int client_fd)
{
    Client client = clients[client_fd];
    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);
    
    std::string message = buffer.substr(0, buffer.length());
    buffer.erase(0, message.length() + 1);
    
    std::vector<std::string> list_channels = split_input(channel_name);
    
    if (channel_name.empty())
        return (send_message(client_fd, ":" + client.get_ip() + " " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " PART :Not enough parameters\r\n", 1));
    for (size_t i = 0; i < list_channels.size(); i++)
    {
        bool found = false;
        for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end();it++)
        {
            if (it->get_name() == list_channels[i])
            {
                found = true;
                
                std::string reply = ":" + client.get_nickname() + "!" + client.get_usrname() + "@" + client.get_ip() + " PART " + list_channels[i] + " " + message + "\r\n";
                ft_send(client_fd, reply.c_str(), reply.length(), 0);
                it->send_toAll(reply, client_fd);
                if (it->is_admin(client) == true && it->get_clients().size() > 1)
                {
                    it->remove_client(client);
                    if(!it->has_admins())
                    {
                        it->add_admins(it->get_clients()[0].get_nickname());
                        it->remove_user(it->get_clients()[0].get_nickname());
                        reply = ":IRC-localhost MODE " + it->get_name() + " -o " + client.get_nickname() + "\r\n";
                        
                        it->send_toAll(reply, client_fd);
                        reply = ":IRC-localhost MODE " + it->get_name() + " +o " + it->get_clients()[0].get_nickname() + "\r\n";
                        
                        it->send_toAll(reply, client_fd);
                        reply = ":IRC-localhost " + std::string(RPL_YOUREOPER) + " " + it->get_clients()[0].get_nickname() + " :You are now an IRC operator\r\n";
                        ft_send(get_fd_by_nickname(it->get_clients()[0].get_nickname()), reply.c_str(), reply.length(), 0);
                    }
                }
                else
                    it->remove_client(client);
                if (it->get_clients().size() == 0)
                        channels.erase(it);
                client.remove_channel(list_channels[i]);
                break ;
            }
        }
        if (found == false)
        {
            if (this->check_channel_if_exists(list_channels[i]) == false)
            {
                std::string reply = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + list_channels[i] + " :No such channel\r\n";
                ft_send(client_fd, reply.c_str(), reply.length(), 0);
                
            }
            else
            {
                std::string reply = ":" + client.get_ip() + " " + std::string(ERR_NOTONCHANNEL) + " " + client.get_nickname() + " " + list_channels[i] + " :You're not on that channel\r\n";
                ft_send(client_fd, reply.c_str(), reply.length(), 0);
            }
        }
        
    }
    return (0);
}