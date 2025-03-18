/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:48:58 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:47:11 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

int Server::privmsg_cmd(std::string buffer, int client_fd)
{
    Client client = clients[client_fd];
    
    std::string target = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, target.length() + 1);
    
    std::string message = buffer.substr(0, buffer.length());
    buffer.erase(0, message.length() + 1);
    
    if (target.empty() || message.empty())
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " :need more parameters\r\n", 1));
    if (message[0] != ':')
        message = message.substr(0, message.find(" "));
    if (message[0] == ':')
        message.erase(0, 1);
    if (message.empty())
        return (send_message(client_fd, ":IRC-localhost " + std::string(ERR_NOTEXTTOSEND) + " " + client.get_nickname() + " :No text to send\r\n", 1));
    std::vector<std::string> targets = split_input(target);
    for (size_t i = 0; i < targets.size(); ++i)
    {
        std::map<int, Client>::iterator it = clients.begin();
        while (it != clients.end())
        {
            if (it->second.get_nickname() == targets[i])
                return (send_message(it->first, ":" + client.get_nickname() + + "!~" + client.get_usrname() + "@" + client.get_ip() + " PRIVMSG " + it->second.get_nickname() + " :" + message + "\r\n", 0));
            it++;
        }
        std::vector<Channel>::iterator it2 = channels.begin();
        while (it2 != channels.end())
        {
            if (it2->get_name() == targets[i])
            {
                std::string reply = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " PRIVMSG " + it2->get_name() + " :" + message + "\r\n";
                std::vector<Client> tmp = it2->get_clients();
                std::vector<Client>::iterator it3 = tmp.begin();
                while (it3 != tmp.end())
                {
                    if (it3->get_client_fd() != client_fd)
                    {
                        ft_send(it3->get_client_fd(), reply.c_str(), reply.length(), 0);
                    }
                    it3++;
                }
                return (0);
            }
            it2++;
        }
        if (it == clients.end() && it2 == channels.end())
            return (send_message(client_fd, ":IRC-localhost "  + std::string(ERR_NOSUCHNICK) + " " + client.get_nickname() + " " + target + " :No such nick/channel\r\n", 1));
    }
    return (0);
}