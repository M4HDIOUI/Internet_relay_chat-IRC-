/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:48:41 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:42:09 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include "utils.hpp"
void   Server::invite_mode(bool mode, std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
        if (check_channel_if_exists(channel_name) == false)
        {
            std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
            ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
            return;
        }
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (!it->is_admin(client))
            {
                std::string message = ":IRC-localost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " : You're not channel operator\r\n";
                ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                return;
            }
            else
            {
                if (mode)
                {
                    it->set_private(true);
                    std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " +i" + "\r\n";
                    it->sendToAll(message);
                    return;
                }
                else
                {
                    it->set_private(false);
                    std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " -i" + "\r\n";
                    it->sendToAll(message);
                    return;
                }
                
            }
        }
    }
}

void    Server::key_mode(std::string mode_arg, bool mode, std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
    std::vector<Channel> &_channels = channels;
    std::vector<Channel>::iterator it = _channels.begin();
    for (it  =_channels.begin(); it != _channels.end(); it++)
    {
        if (check_channel_if_exists(channel_name) == false)
        {
            std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
            ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
            return;
        }
        
        else if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (!it->is_admin(client))
            {
                std::string message = ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " : You're not channel operator\r\n";
                ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                return;
            }
            if (mode)
            {
                if (mode_arg.empty())
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
                    return;
                }
                it->set_password(mode_arg);
                std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " +k " + mode_arg + "\r\n";
                it->sendToAll(message);
                return;
            }
            else
            {
                it->set_password("");
                std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " -k " + "\r\n";
                it->sendToAll(message);
                return;
            }
        }
    }
    if (it == _channels.end())
    {
        std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
        ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
        return;
    }
}

void   Server::limit_mode(int limit, bool mode, std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
    std::vector<Channel> &_channels = channels;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if(check_channel_if_exists(channel_name) == false)
        {
            std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
            ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
            return;
        }
        else if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (!it->is_admin(client))
            {
                std::string message = ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " : You're not channel operator\r\n";
                ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                return;
            }
            if (mode)
            {
                if (limit < 1)
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
                    return;
                }
                it->set_limit(limit);
                std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " +l " + std::to_string(limit) + "\r\n";
                it->sendToAll(message);
                return;
            }
            else
            {
                it->set_limit(limit);
                std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " -l " + "\r\n";
                it->sendToAll(message);
                return;
            }
        }
    }
}

void   Server::topic_mode(bool mode, std::string mode_arg, std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
    std::vector<Channel> &_channels = channels;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if (check_channel_if_exists(channel_name) == false)
        {
            std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
            ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
            return;
        }
        else if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (!it->is_admin(client))
            {
                std::string message = ":IRC-localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " : You're not channel operator\r\n";
                ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                return;
            }
            if (mode)
            {
                if (mode_arg.empty())
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " MODE :Not enough parameters\r\n";
                    ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
                    return;
                }
                it->set_topic(mode_arg);
                std::string message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " +t " + mode_arg + "\r\n";
                it->sendToAll(message);
                return;
            }
            else
            {
                if (mode_arg.empty())
                {
                    std::string message = "";
                    message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " +t " + "\r\n";
                    ft_send(it->get_clients()[0].get_client_fd(), message.c_str(), message.length(), 0);
                    return;
                }
                it->set_topic("");
                std::string message = "";
                message = ":" + client.get_nickname() + "!~" + client.get_usrname() + "@" + client.get_ip() + " MODE " + channel_name + " -t " + "\r\n";
                it->sendToAll(message);
                return;
            }
        }
    }
}

void    Channel::add_admins(std::string nickname) 
{
    for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (*it == nickname)
        {
            users.erase(it);
            break;
        }
    }
    admins.push_back(nickname);
}

void    Channel::remove_admins(std::string nickname)
{
    for (std::vector<std::string>::iterator it = admins.begin(); it != admins.end(); it++)
    {
        if (*it == nickname)
        {
            admins.erase(it);
            break;
        }
    }
    users.push_back(nickname);
}

void   Server::op_mode(std::string mode_arg, bool mode, std::string channel_name, int client_fd)
{
    Client client = clients[client_fd];
    std::vector<Channel> &_channels = channels;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if (strcmp(it->get_name().c_str(), channel_name.c_str()) == 0)
        {
            if (it->has_client(mode_arg))
            {
                if (!it->is_admin(client))
                {
                    std::string message = ":IRC-localost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + client.get_nickname() + " " + channel_name + " MODE : You're not channel operator\r\n";
                    ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                    return;
                }
                if (mode)
                {
                    it->add_admins(mode_arg);
                    std::string message = ":IRC-localhost MODE " + channel_name + " +o " + mode_arg + "\r\n";
                    it->sendToAll(message);
                }
                else
                {
                    it->remove_admins(mode_arg);
                    std::string message = ":IRC-localhost MODE " + channel_name + " -o " + mode_arg + "\r\n";
                    it->sendToAll(message);
                    
                }
            }
            else
            {
                std::string message = ":IRC-localhost " + std::string(ERR_USERNOTINCHANNEL) + " " + client.get_nickname() + " " + mode_arg + " :No such nick/channel\r\n";
                ft_send(client.get_client_fd(), message.c_str(), message.length(), 0);
                return;
            }
        }
    }
}

int   Server::mode_cmd(std::string buffer, int client_fd)
{ 
    Client client = clients[client_fd];
    
    std::string channel_name = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, channel_name.length() + 1);

    std::string mode = buffer.substr(0, buffer.find(" "));
    buffer.erase(0, mode.length() + 1);
    
    std::vector<std::string> mode_arg = split_input(buffer);
    if (channel_name.empty())
    {
        std::string message = "";
        message = ":" + client.get_ip() + " " + std::string(ERR_NEEDMOREPARAMS) +" MODE :Not enough parameters\r\n";
        ft_send(client_fd, message.c_str(), message.length(), 0);
        return (1);
    }
    
    if (mode.empty()) 
    {
        std::string modes = this->get_modes_string(channel_name);
        if (!modes.empty()) 
        {
            std::string message = ":IRC-localhost " + std::string(RPL_CHANNELMODEIS) + " " + client.get_nickname() + " " + channel_name + " " + modes + "\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
        }
        else 
        {
            std::string message = ":IRC-localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + client.get_nickname() + " " + channel_name + " :No such channel\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
        }
        return 0;
    }

    int i = 0;
    int j = 0;
    bool dd;
    std::string op;
    while(mode[i])
    {
        if (mode[i] == '+')
            dd = true;
        else if (mode[i] == '-')
            dd = false;
        else if (mode[i] == 'i')
        {
            if (dd == true)
                op = "+i";
            else if (dd == false)
                op = "-i";
            if (strcmp(op.c_str(), "+i") == 0)
                invite_mode(true, channel_name, client_fd);
            else if (strcmp(op.c_str(), "-i") == 0)
                invite_mode(false, channel_name, client_fd);
        }
        else if (mode[i] == 'k')
        {
            if (dd == true)
                op = "+k";
            else if (dd == false)
                op = "-k";
            if (mode_arg.empty())
            {
                if (op == "-k")
                    key_mode("", dd, channel_name, client_fd);
                else
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(client_fd, message.c_str(), message.length(), 0);
                    return (1);
                }
            }
            else
                key_mode(mode_arg[j], dd, channel_name, client_fd);
            if (op == "+k")
                j++;
        }
        else if (mode[i] == 'l')
        {
            if (dd == true)
                op = "+l";
            else if (dd == false)
                op = "-l";
            if (mode_arg.empty())
            {
                if (op == "-l")
                    limit_mode(INT_MAX, dd, channel_name, client_fd);
                else
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(client_fd, message.c_str(), message.length(), 0);
                    return (1);
                }
            }
            else
                limit_mode(atoi(mode_arg[j].c_str()), dd, channel_name, client_fd);
            if (op == "+l")
                j++;
        }
        else if (mode[i] == 't')
        {
            if (dd == true)
                op = "+t";
            else if (dd == false)
                op = "-t";
            if (mode_arg.empty())
            {
                if (op == "-t")
                    topic_mode(false, "", channel_name, client_fd);
                else
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(client_fd, message.c_str(), message.length(), 0);
                    return (1);
                }
            }
            else if (op == "+t")
                topic_mode(true, mode_arg[j], channel_name, client_fd);
            if (op == "+t")
                j++;
        }
        else if (mode[i] == 'o')
        {
            if (dd == true)
                op = "+o";
            else if (dd == false)
                op = "-o";
            if (op == "+o" || op == "-o")
            {
                if (mode_arg.empty())
                {
                    std::string message = "";
                    message = ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + client.get_nickname() + " MODE :Not enough parameters\r\n";
                    ft_send(client_fd, message.c_str(), message.length(), 0);
                    return (1);
                }
                else
                    op_mode(mode_arg[j], dd, channel_name, client_fd);
            }
            j++;
        }
        else
        {
            std::string message = "";
            message = ":IRC-localhost "  + std::string(ERR_UNKNOWNMODE) + " " + client.get_nickname() +  " "+ mode[i] + " :is unknown mode char to me for\r\n";
            ft_send(client_fd, message.c_str(), message.length(), 0);
        }
        i++;
    }
    return (0);
}