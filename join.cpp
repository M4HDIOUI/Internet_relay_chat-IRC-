/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:47:16 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 03:10:49 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include "client.hpp"
#include "utils.hpp"

void Server::join_message(Channel channel, int socker_fd, std::string channel_name)
{
    std::string reply = "";
    reply = ":" +  clients[socker_fd].get_nickname()+"!~"+ clients[socker_fd].get_usrname() + "@" + clients[socker_fd].get_ip() + " JOIN " + channel_name + "\r\n";
    ft_send(socker_fd, reply.c_str(), reply.length(), 0);
    reply = ":IRC-localhost MODE " + channel_name + " " + get_modes_string(channel_name) + "\r\n";
    ft_send(socker_fd, reply.c_str(), reply.length(), 0);
    
    std::string message = ":IRC-localhost " + std::string(RPL_NAMREPLY) + " " + clients[socker_fd].get_nickname() + " = " + channel_name + " :" + channel.get_client_nickname() + "\r\n";
    ft_send(socker_fd, message.c_str(), message.length(), 0);

    std::string reply2 = ":IRC-localhost " + std::string(RPL_ENDOFNAMES) + " " + clients[socker_fd].get_nickname() + " " + channel_name + " :End of /NAMES list\r\n";
    ft_send(socker_fd, reply2.c_str(), reply2.length(), 0);
    
    std::string message1 = ":" +  clients[socker_fd].get_nickname()+"!~"+ clients[socker_fd].get_usrname() + "@" + clients[socker_fd].get_ip() + " JOIN " + channel_name + "\r\n";
    channel.send_toAll(message1, socker_fd);
}

int Server::join_cmd(std::string buffer, int socker_fd)
{
    Client client = clients[socker_fd];

    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);
    
    std::string key = buffer.substr(0, buffer.length());
    
    std::vector<std::string> list_channels = split_input(channel_name);
    std::vector<std::string> keys = split_input(key);
    
    if (list_channels.empty())
        return (send_message(socker_fd, ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " :Not enough parameters !\r\n", 1));

    for (size_t i = 0; i < list_channels.size() ; ++i)
    {
        if (list_channels[i][0] == '0' || list_channels[i].size() == 1)
        {
            for (size_t j = 0; j < clients[socker_fd].get_channels().size(); ++j)
            {
                part_cmd(clients[socker_fd].get_channels()[j], socker_fd);
            }
            return 0;
        }
        if (list_channels[i][0] != '#' || list_channels[i].size() == 1)
        {
            std::string reply = ":IRC-localhost " + std::string(ERR_BADCHANMASK) + " " + client.get_nickname() + " " + list_channels[i] + " :bad channel name.\r\n";
            ft_send(socker_fd, reply.c_str(), reply.length(), 0);
            break;
        }
        else if (check_channel_if_exists(list_channels[i]) == false)
        {
            Channel new_channel(list_channels[i]);
            if (!keys.empty() && i < keys.size()) 
            {
                new_channel.set_password(keys[i]);
            }
            new_channel.add_client(client, true);
            channels.push_back(new_channel);
            clients[socker_fd].add_channel(list_channels[i]);
            join_message(new_channel, socker_fd, list_channels[i]);
        }
        else
        {
            for (std::vector<Channel>::iterator ch = channels.begin(); ch != channels.end(); ch++)
            {
                if (ch->get_name() == list_channels[i])
                {
                    if (ch->check_join(*ch, socker_fd, client, i < keys.size() ? keys[i] : "") == 1)
                    {
                        break;
                    }
                    ch->add_client(client, false);
                    join_message(*ch, socker_fd, list_channels[i]);
                    clients[socker_fd].add_channel(list_channels[i]);
                    break;
                }
            }
        }
    }
    return 0;
}