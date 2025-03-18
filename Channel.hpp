/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:46:31 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:40:40 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "client.hpp"
# include "numeric_replies.hpp"
# include <iostream>

ssize_t ft_send(int socket, const void *buffer, size_t length, int flags);
int send_message(int client_sock, std::string message, int return_value);


class Client;
class Server;

class Channel {
    private:
        std::string name;
        std::string password;
        std::string topic; 
        std::vector<Client> join_clients;
        std::string paswrd;
        std::vector<std::string> admins;
        std::vector<std::string> users;
        std::vector<std::string> invited;
        std::string tnickname;
        bool is_private;
        size_t limit;
        std::string mode_string;
    public:
        Channel();
        Channel(const std::string& which_name);
        
        std::string get_name() const;
        std::vector<Client>& get_clients();
        bool get_private() const;
        size_t get_limit() const;
        std::string get_client_nickname() const;
        std::string get_password() const;
        std::vector<std::string> get_admins() const;
        std::vector<std::string> get_invited() const;                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        std::vector<std::string>& get_users();
        std::string get_topic() const;

        void set_private(bool is_private);
        void set_limit(int limit);
        void set_topic(std::string topic);
        void set_password(std::string password);
        
        std::string get_mode_string() const;
        bool check_invited(std::string nickname) const;
        bool is_admin(const Client& client) const;
        bool is_admin(std::string nickname) const;
        void remove_client(std::string nickname);
        void send_toAll(const std::string& message, int exclude_fd);
        void remove_admins(std::string nickname);
        void add_admins(std::string nickname);
        void add_client(Client client, bool is_admin = false);
        void remove_client(const Client& client);
        bool has_client(const Client& client) const;
        void remove_user(std::string nickname);
        bool add_invited(const std::string& nickname);
        bool has_client(std::string nickname) const;
        void sendToAll(const std::string& message);
        int check_join(Channel channel, int client_fd, Client client, std::string password);
        int kick_check(Channel channel, int client_fd, Client client, std::string user, bool i);
        void set_topic_name(std::string tnickname);
        bool has_admins() const;
};
#endif