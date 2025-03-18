/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:50:05 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 03:10:52 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "numeric_replies.hpp"

ssize_t ft_send(int socket, const void *buffer, size_t length, int flags)
{
    return (send(socket, buffer, length, flags));
}

int send_message(int client_sock, std::string message, int return_value)
{
    ft_send(client_sock, message.c_str(), message.length(), 0);
    return (return_value);
}

void Server::handleClientDisconnection(int client_fd) 
{
    close (client_fd);
    for (size_t i = 0; i < fds_clients.size(); ++i) 
    {
        if (fds_clients[i].fd == client_fd) 
        {
            fds_clients.erase(fds_clients.begin() + i);
            break;
        }
    }
}

void Server::fill_client(int client_fd, char *client_ip)
{
    clients[client_fd].set_client_fd(client_fd);
    clients[client_fd].set_registered(false);
    clients[client_fd].set_pass_check(false);
    clients[client_fd].set_nick_check(false);
    clients[client_fd].set_user_check(false);
    clients[client_fd].set_ip(client_ip);
}

int Server::count_spaces(const std::string& str, int start) 
{
    int count = 0;
    for (size_t i = start; i < str.size(); ++i) 
    {
        if (str[i] == ' ')
            ++count;
        else
            break;
    }
    return count;
}

int is_space(std::string str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == ' ' || str[i] == '\t')
            return 0;
    }
    return 1;
}

Server::Server(std::string port, std::string passwd)
{
    this->port = atoi(port.c_str());
    if (this->port < 1024 || this->port > 65535)
    {
        throw std::invalid_argument("Invalid port number");
    }
    if (is_space(passwd) == 0)
    {
        throw std::invalid_argument("Invalid password : do not use spaces in password");
    }
    this->password = passwd;
    clients.clear();
    channels.clear();
    fds_clients.clear();
    this->run();
    input_f = "";
}

int Server::initialize_server()
{
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_addr.s_addr = INADDR_ANY; 
    sa.sin_family = AF_INET; 
    sa.sin_port = htons(this->port);
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        return -1;
    }
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        close(server_fd);
        return -1;
    }
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: fcntl failed" << std::endl;
        close(server_fd);
        return -1;
    }
    if (bind(server_fd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        std::cerr << "Error: bind failed" << std::endl;
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, SOMAXCONN) == -1) 
    {
        std::cerr << "Error: listen failed" << std::endl;
        close(server_fd);
        return -1;
    }
    struct pollfd new_client;
    new_client.fd = server_fd;
    new_client.events = POLLIN;
    new_client.revents = 0;
    this->fds_clients.push_back(new_client);

    return server_fd;
}



int Server::accept_connection(int server_fd)
{
    if(poll(this->fds_clients.data(), this->fds_clients.size(), -1) == -1)
    {
        std::cerr << "Error: poll failed" << std::endl;
        close(server_fd);
        return -1;
    }
    for (size_t i = 0; i < this->fds_clients.size(); i++)
    {
        if (this->fds_clients[i].revents & POLLIN)
        {
            int client_fd = 0;
            if (this->fds_clients[i].fd == server_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                
                client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (client_fd == -1)
                {
                    perror("accept");
                    close(server_fd);
                    return -1;
                }
                
                if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
                {
                    std::cerr << "Error: fcntl failed" << std::endl;
                    close(client_fd);
                    return -1;
                }
                struct pollfd new_client;
                new_client.fd = client_fd;
                new_client.events = POLLIN;
                new_client.revents = 0;
                this->fds_clients.push_back(new_client);
                this->fill_client(client_fd, inet_ntoa(client_addr.sin_addr));
                std::cout << "N E W  C L I E N T  C O N N E C T E D " << client_fd << std::endl;
            }
            else
            {
                char buffer[1024];
                int bytes_read = recv(this->fds_clients[i].fd, buffer, 1024, 0);
                if (bytes_read == -1)
                {
                    perror("recv");
                    close(this->fds_clients[i].fd);
                    this->fds_clients.erase(this->fds_clients.begin() + i);
                }
                if (bytes_read == 0)
                {
                    this->quit_cmd(this->fds_clients[i].fd);
                    return 0;
                }
                buffer[bytes_read] = '\0';
                std::string buffer_str = buffer;
                if (buffer_str.find("\r\n") != std::string::npos)
                {
                    std::string tmp = input_f + buffer_str.substr(0, buffer_str.find("\r\n"));
                    handle_input(this->fds_clients[i].fd, tmp);
                }
                else if (buffer_str.find("\n") != std::string::npos)
                {
                    std::string tmp = input_f + buffer_str.substr(0, buffer_str.find("\n"));
                    handle_input(this->fds_clients[i].fd, tmp);
                }
                else
                {
                    input_f += buffer_str;
                    handle_input(this->fds_clients[i].fd, std::string());
                }
            }
        }
    }
    return 0;
}

void Server::run()
{
    int server_fd = this->initialize_server(); 
    if (server_fd == -1)
    {
        std::cerr << "Error: server initialization failed" << std::endl;
        return;
    }
    while (true)
    {
        this->accept_connection(server_fd);
    }
    this->clear_data(server_fd);
}

int Server::handle_input(int socket_fd, std::string buffer)
{
    Client added_client = clients[socket_fd];
    if (buffer.size() <= 0)
        return 0;
    this->input_f = "";
    buffer.erase(buffer.find_last_not_of(" \n\r\t") + 1);
    std::vector<std::string> a = this->parse_input(buffer);
    std::string cmd = a[0];

    std::string tmp_buffer = a[1];
    if (cmd == "PASS" || cmd == "NICK" || cmd == "USER")
    {
        register_pars(cmd, buffer, socket_fd);
    }
    else if (strcmp(cmd.c_str() , "JOIN") == 0 || strcmp(cmd.c_str() , "INVITE") == 0 || strcmp(cmd.c_str() , "KICK") == 0 || strcmp(cmd.c_str(), "MODE") == 0 || strcmp(cmd.c_str(), "PART") == 0 || strcmp(cmd.c_str(), "PRIVMSG") == 0 || strcmp(cmd.c_str(), "TOPIC") == 0 || strcmp(cmd.c_str(), "LIST") == 0 || strcmp(cmd.c_str() , "QUIT") == 0)
    {
        if (clients[socket_fd].get_registered() == 0)
            return (send_message(socket_fd, ":IRC-localhost " + std::string(ERR_NOTREGISTERED) + " :You have not registered !\r\n", -1));
        if (cmd == "QUIT")
        {   
            quit_cmd(socket_fd);
        }
        else if (strcmp(cmd.c_str() , "JOIN") == 0) 
        {
            join_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str() , "INVITE") == 0)
        {
            invite_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str() , "KICK") == 0)
        {
            kick_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str(), "MODE") == 0)
        {
            mode_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str(), "PART") == 0)
        {
            part_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str(), "PRIVMSG") == 0)
        {
            privmsg_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str(), "TOPIC") == 0)
        {
            topic_cmd(tmp_buffer, socket_fd);
        }
        else if (strcmp(cmd.c_str(), "LIST") == 0)
        {
            list_cmd(tmp_buffer, socket_fd);
        }
    }
    else
        return (send_message(socket_fd, ":IRC-localhost " + std::string(ERR_UNKNOWNCOMMAND) + " " + added_client.get_nickname() + " :Unknown command\r\n", -1));
    return 0;
}

void Server::clear_data(int socket_fd)
{
    this->handleClientDisconnection(socket_fd); 
}

std::vector<std::string> Server::parse_input(std::string buffer)
{
    std::vector<std::string> tokens;
    
    size_t i = 0;
    while (buffer[i] == ' ' || buffer[i] == '\t')
        i++;
    buffer.erase(0, i);
    std::string tmp_buffer = buffer;
    std::string tmp_cmd = tmp_buffer.substr(0, tmp_buffer.find(" "));
    std::string cmd = tmp_cmd;
    tmp_buffer.erase(0, tmp_cmd.size() + count_spaces(tmp_buffer, tmp_cmd.size()));
    for (unsigned long i = 0; i < tmp_cmd.length(); i++)
        cmd[i] =  (char)toupper(cmd[i]);
    tokens.push_back(cmd);
    tokens.push_back(tmp_buffer);
    return tokens;
}

int Server::get_port()
{
    return (port);
}

std::string Server::get_password()
{
    return (password);
}

void Server::add_channel(const Channel& channel)
{
    channels.push_back(channel);
}

bool Server::check_channel_if_exists(std::string channel_name)
{
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (strcmp(channels[i].get_name().c_str(), channel_name.c_str()) == 0)
        {
            return true;
        }
    }
    return false;
}

std::map<int, Client>& Server::get_clients()
{
    return clients;
}

bool Server::check_client_if_exists(const std::string& nickname) const 
{
    for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); it++) 
    {
        if (it->second.get_nickname() == nickname) 
        {
            return true;
        }
    }
    return false;
}

Channel Server::get_channel(std::string channel_name)
{
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i].get_name() == channel_name)
        {
            return channels[i];
        }
    }
    return Channel();
}

std::string Server::get_modes_string(const std::string& channel_name) {
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->get_name() == channel_name) {
            std::string mode_string;
            std::string mode_args;

            if (it->get_private()) {
                mode_string += "i";
            }

            if (!it->get_password().empty()) {
                mode_string += "k";
                mode_args += " " + it->get_password();
            }

            int limit = it->get_limit();
            if (limit > 0) {
                mode_string += "l" ;
                mode_args +=  " " + std::to_string(limit);
                
            }

            if (!it->get_topic().empty()) {
                mode_string += "t";
                mode_args += " " + it->get_topic();
            }

            return ("+o" + mode_string + " " + mode_args);
        }
    }
    return "";
}