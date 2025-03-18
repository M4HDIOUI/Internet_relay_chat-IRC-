/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahil <sahil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:50:09 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 19:56:58 by sahil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include<iostream>
#include<sstream>
#include<libc.h>
#include<vector> 
#include<map>
#include<poll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/poll.h>
#include<sys/types.h>

#include "Channel.hpp"
#include "client.hpp"
#include "utils.hpp"
#include "numeric_replies.hpp"

ssize_t ft_send(int socket, const void *buffer, size_t length, int flags);
int send_message(int client_sock, std::string message, int return_value);


class Client;
class Channel;


typedef struct poll_stc {
    int fd;
    short events;
    short revents;
} poll_stc;




class Server {
    private:
        int port;
        std::string password;
              
        std::string input_f;
        std::map<int, Client> clients;
        std::vector<Channel> channels;
        std::vector<struct pollfd> fds_clients;
    public:
        Server(std::string port, std::string passwd);
        void run();
        int initialize_server();
        int accept_connection(int server_fd);
        void fill_client(int client_fd, char *ip);

        
        int nick_exists(std::string nick);
        int register_pars(std::string cmd, std::string buffer, int client_sock); 
        int get_port();
        std::string get_password();
        Channel get_channel(std::string channel_name);
        std::map<int, Client>& get_clients();
        int get_fd_by_nickname(std::string nickname);
        void get_topic(std::string channel_name, int socket_fd);
        
        int join_cmd(std::string buffer, int socket_fd);
        int invite_cmd(std::string buffer, int socker_fd);
        int kick_cmd(std::string buffer, int socket_fd);
        int mode_cmd(std::string buffer, int socket_fd);
        int quit_cmd(int socket_fd);
        int part_cmd(std::string buffer, int socket_fd);
        int privmsg_cmd(std::string buffer, int socket_fd);
        int list_cmd(std::string buffer, int client_fd);
        void invite_mode(bool mode, std::string channel_name, int socket_fd);

        
        void key_mode(std::string mode_arg, bool mode, std::string channel_name, int socket_fd);
        void limit_mode(int limit, bool mode, std::string channel_name, int socket_fd);
        void topic_mode(bool mode, std::string mode_arg, std::string channel_name, int socket_fd);
        void op_mode(std::string mode_arg, bool mode, std::string channel_name, int socket_fd);
        
        int  handle_input(int socket_fd, std::string buffer);
        void add_channel(const Channel& channel);
        void handleClientDisconnection(int client_fd);
        bool check_channel_if_exists(std::string channel_name);
        bool check_client_if_exists(const std::string& nickname) const;
        void join_message(Channel channel, int socket_fd, std::string channel_name);
        int  topic_cmd(std::string buffer, int socket_fd);

        void unset_topic(std::string channel_name, int socket_fd);
        void set_topic(std::string channel_name, std::string buffer, int socket_fd);
        int count_spaces(const std::string& str, int start);
        void clear_data(int socket_fd);
        std::vector<std::string> parse_input(std::string buffer);
        std::string get_modes_string(const std::string& channel_name);
        std::string check_linend(std::string buffer, int socket_fd);

};


#endif