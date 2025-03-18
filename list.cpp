/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:47:32 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 04:47:43 by omahdiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include "client.hpp"

int Server::list_cmd(std::string buffer, int client_fd)
{
    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);
    if (channel_name.empty())
    {
        std::string reply = "";
        for (size_t i = 0; i < channels.size(); i++)
        {
            reply = ":" + clients[client_fd].get_ip() + " " + std::string(RPL_LIST) + " " + clients[client_fd].get_nickname() + " " + channels[i].get_name() + " " + std::to_string(channels[i].get_clients().size()) + " :" + channels[i].get_topic() + "\r\n";
            ft_send(client_fd, reply.c_str(), reply.length(), 0);
        }
        reply = ":" + clients[client_fd].get_ip() + " " + std::string(RPL_LISTEND) + " " + clients[client_fd].get_nickname() + " :End of /LIST\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
    }
    else
    {
        std::string reply = "";
        reply = ":" + clients[client_fd].get_ip() + " " + std::string(RPL_LISTSTART) + " " + clients[client_fd].get_nickname() + " " + channel_name + "\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
        reply = ":" + clients[client_fd].get_ip() + " " + std::string(RPL_LIST) + " " + clients[client_fd].get_nickname() + " " + channel_name + " " + std::to_string(get_channel(channel_name).get_clients().size()) + " :" + get_channel(channel_name).get_topic() + "\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
        reply = ":" + clients[client_fd].get_ip() + " " + std::string(RPL_LISTEND) + " " + clients[client_fd].get_nickname() + " " + channel_name + " :End of /LIST\r\n";
        ft_send(client_fd, reply.c_str(), reply.length(), 0);
    }
    return (0);
} 