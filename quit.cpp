/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:49:18 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 03:10:55 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int Server::quit_cmd(int client_fd)
{
    std::cout << "C L I E N T  D I S C O N N E C T E D " << client_fd << std::endl;

    Client client = clients[client_fd];
    std::string reply =  ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " QUIT :Client Quit\r\n";
    ft_send(client_fd, reply.c_str(), reply.length(), 0);
    for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->has_client(client))
        {
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
        }
    }
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (it->first == client_fd)
        {
            clients.erase(it);
            break ;
        }
    }
    handleClientDisconnection(client_fd);
    return (0);
}