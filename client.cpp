/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:46:44 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:45:26 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int client_fd)
{
    this->client_fd = client_fd;
    this->ip = "";
    this->password = "";
    this->usrname = "";
    this->nickname = "";
    this->port = 0;
    channels.clear();
    set_pass_check(false);
    set_nick_check(false);
    set_user_check(false);
    set_registered(false);
}

void Client::set_pass_check(bool new_pass_check)
{
    this->register_check.pass_check = new_pass_check;
}

void Client::set_nick_check(bool new_nick_check)
{
    this->register_check.nick_check = new_nick_check;
}

void Client::set_user_check(bool new_user_check)
{
    this->register_check.user_check = new_user_check;
}

void Client::set_registered(bool new_is_registered)
{
    this->register_check.is_registered = new_is_registered;
}

bool Client::get_pass_check() const
{
    return (register_check.pass_check);
}

bool Client::get_nick_check() const
{
    return (register_check.nick_check);
}
std::vector<std::string> Client::get_channels() const
{
    return (channels);
}
bool Client::get_user_check() const
{
    return (register_check.user_check);
}

bool Client::get_registered() const
{
    return (register_check.is_registered);
}

std::string Client::get_usrname() const
{
    return (usrname);
}

std::string Client::get_nickname() const
{
    return (nickname);
}

int Client::get_port() const
{
    return (port);
}

std::string Client::get_password() const
{
    return (password);
}

int Client::get_client_fd() const
{
    return (client_fd);
}

sockaddr_in Client::get_server_address() const
{
    return (server_address);
}

void Client::set_usrname(const std::string& new_usrname)
{
    this->usrname = new_usrname;
    if (!usrname.empty() && *usrname.rbegin() == '\n') 
    {
        usrname.erase(usrname.size() - 1);
    }
}

void Client::set_nickname(const std::string& new_nickname)
{
    this->nickname = new_nickname;
    if (!nickname.empty() && *nickname.rbegin() == '\n') 
    {
        nickname.erase(nickname.size() - 1);
    }
}

void Client::set_port(int new_port)
{
    this->port = new_port;
}

void Client::set_password(const std::string& new_password)
{
    this->password = new_password;
    if (!password.empty() && *password.rbegin() == '\n') 
    {
        password.erase(password.size() - 1);
    }
}

void Client::set_client_fd(int new_socket_fd)
{
    this->client_fd = new_socket_fd;
}

void Client::set_server_address(sockaddr_in new_server_address)
{
    this->server_address = new_server_address;
}

bool Client::operator==(const Client& client) const
{
    return (this->client_fd == client.get_client_fd());
}

Client Client::get_client() const
{
    return (*this);
}

void Client::set_realname(std::string realname)
{
    this->realname = realname;
}

std::string Client::get_realname() const
{
    return (realname);
}

void Client::set_ip(std::string ip)
{
    this->ip = ip;
}

std::string Client::get_ip() const
{
    return (ip);
}

void Client::add_channel(const std::string channel_name)
{
    channels.push_back(channel_name);
}

void Client::remove_channel(const std::string channel_name)
{
    std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channel_name);
    if (it != channels.end())
    {
        channels.erase(it);
    }
}