/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:49:31 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:45:59 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Channel.hpp"
#include <unistd.h>

std::string remove_spaces(std::string buffer)
{
    std::string n_buff;

    for (int i = buffer.length() - 1; i >= 0; i--)
    {
        if (buffer[i] != ' ' && buffer[i] != '\t')
        {
            n_buff = buffer.substr(0, i + 1);
            break;
        }
    }
    return (n_buff);
}

int Server::nick_exists(std::string nick)
{
    std::map<int, Client>::iterator it = this->clients.begin();
    if (it == this->clients.end())
        return (1);
    while (it != this->clients.end())
    {
        if (strcmp(it->second.get_nickname().c_str(), nick.c_str()) == 0)
            return (-1);
        it++;
    }
    return (1);
}

std::vector<std::string> split_arg(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}


int isvalid(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (std::isspace(str[i]) || str[i] == '#' || str[i] == '?' || str[i] == ',' || str[i] == '&' || str[i] == ':' || str[i] == '.' || str[i] == '\r' || str[i] == '\n' || str[i] == ' ' || str[i] == '\t' || str[i] == '!' || str[i] == ';' || str[i] == '@')
            return (-1);
    }
    return (1);
}

int check_user_isvalid(std::string str)
{
    if (str.length() < 1)
        return (-1);
    if (str[0] == ':' || str[0] == '!' || str[0] == '@')
        return (-1);
    return (1);
}



int Server::register_pars(std::string cmd, std::string buffer, int client_sock)
{
        std::string args;
        int t = 0;

        for (size_t i = 0; i < buffer.length(); i++)
        {
            if (buffer[i] == ' ' || buffer[i] == '\t')
               t++;
            else
                break;
        }
        buffer.erase(0, t);
        if (buffer.length() < 5)
            return (send_message(client_sock, "IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters !\r\n", -1));
        args = buffer.substr(5);
        if (cmd == "PASS")
        {
            if (this->clients[client_sock].get_registered() == true)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_ALREADYREGISTERED) + " :You are already connected and cannot handshake again !\r\n", -1));

            else if (this->clients[client_sock].get_pass_check() == true)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_ALREADYREGISTERED) + " :you may not register !\r\n", -1));

            else if (args.length() == 0)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters !\r\n", -1));

            else if (strcmp(args.c_str(), this->password.c_str()) != 0)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_PASSWDMISMATCH) + " :Password mismatch !\r\n", -1));

            else
            {
                this->clients[client_sock].set_pass_check(true);
                this->clients[client_sock].set_client_fd(client_sock);
            }
        }
        else if (cmd == "NICK")
        {
            args = args.erase(0, count_spaces(args, 0));
            args = args.substr(0, args.find(" "));
            args = remove_spaces(args);
            if (this->clients[client_sock].get_pass_check() == false)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_NOTREGISTERED) + " :You have not registered !\r\n", -1));

            else if (args.length() == 0)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given !\r\n", -1));

            else if (isvalid(args) == -1)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_ERRONEUSNICKNAME) + " " + args + " :Erroneus nickname !\r\n", -1));

            else if (nick_exists(args) == -1)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_NICKNAMEINUSE) + " " + args + " :Nickname is already in use !\r\n", -1));
            else
            {
                if (this->clients[client_sock].get_registered() == true)
                {
                    send_message(client_sock, ":" + this->clients[client_sock].get_nickname() + "!" + this->clients[client_sock].get_usrname() + " NICK " + args + "\r\n", 1);
                }
                this->clients[client_sock].set_nick_check(true);
                if (args.length() > 16)
                    args = args.substr(0, 16);
                this->clients[client_sock].set_nickname(args);
            }
        }
        else if (cmd == "USER")
        {
            args = args.erase(0, count_spaces(args, 0));
            args = remove_spaces(args);
            if (this->clients[client_sock].get_registered() == true)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_ALREADYREGISTERED) + " :You are already connected and cannot handshake again !\r\n", -1));

            if (this->clients[client_sock].get_pass_check() == false)
                return (send_message(client_sock, ":IRC-localhost " + std::string(ERR_ALREADYREGISTERED) + " :you may not register !\r\n", -1));
            if (args.length() == 0)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters !\r\n", -1));

            std::vector<std::string> split_args = split_arg(args, " ");
            if (split_args.size() != 4)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters !\r\n", -1));
            else if (check_user_isvalid(split_args[0]) == -1)
                return (send_message(client_sock, "IRC-localhost " + std::string(ERR_ERRONEUSNICKNAME) + " " + args + " :Erroneus nickname !\r\n", -1));
            else if (this->clients[client_sock].get_user_check() == true)
            {
                return (-1);
            }
            else
            {
                this->clients[client_sock].set_user_check(true);
                this->clients[client_sock].set_usrname(split_args[0]);
                this->clients[client_sock].set_realname(split_args[3]);
            }
        }
        if (this->clients[client_sock].get_pass_check() == true && this->clients[client_sock].get_nick_check() == true &&
         this->clients[client_sock].get_user_check() == true)
        {
            this->clients[client_sock].set_user_check(false);
            this->clients[client_sock].set_nick_check(false);
            clients[client_sock].set_registered(true);
            send_message(client_sock, ":IRC-localhost " + std::string(RPL_WELCOME) + " "  +this->clients[client_sock].get_nickname() + " :Welcome to the IRC-localhost Network, " + this->clients[client_sock].get_nickname() + "[!" + this->clients[client_sock].get_usrname() + "@IRC-localhost] !\r\n", 0);
            send_message(client_sock,  ":IRC-localhost " + std::string(RPL_YOURHOST) + " "  +this->clients[client_sock].get_nickname() + " :Your host is IRC-localhost, running version 1.0 !\r\n", 0);
            send_message(client_sock, ":IRC-localhost " + std::string(RPL_CREATED) + " "  +this->clients[client_sock].get_nickname() + " :This server was created 2024-07-24 !\r\n", 0);
            return (send_message(client_sock, ":IRC-localhost " + std::string(RPL_MYINFO) + " "  +this->clients[client_sock].get_nickname() + " :Host: IRC-IRC-localhost, Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n", 1));
        }
    return client_sock;
}