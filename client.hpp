/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmesbahi <mmesbahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:46:56 by omahdiou          #+#    #+#             */
/*   Updated: 2024/09/02 02:46:41 by mmesbahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring>
# include <cstdlib>

typedef struct register_chec {
    bool pass_check;
    bool nick_check;
    bool user_check;
    bool is_registered;
} register_chec;

class Client {
private:
    std::string usrname;
    std::string nickname;
    int port;
    std::string ip;
    register_chec register_check;
    std::string password;
    int client_fd;
    sockaddr_in server_address;
    std::string realname;
    std::vector<std::string> channels;
public:
    Client() : client_fd(-1) {} 
    Client(int client_fd);
    bool operator==(const Client& client) const;
    
    std::string get_usrname() const;
    std::string get_nickname() const;
    int get_port() const;
    std::string get_password() const;
    int get_client_fd() const;
    sockaddr_in get_server_address() const;
    Client get_client() const;
    std::string get_realname() const;
    std::string get_ip() const;

    void set_pass_check(bool new_pass_check);
    void set_nick_check(bool new_nick_check);
    void set_user_check(bool new_user_check);
    void set_registered(bool new_registered);
    bool get_pass_check() const;
    bool get_nick_check() const;
    bool get_user_check() const;
    bool get_registered() const;
    void set_usrname(const std::string& new_usrname);
    void set_nickname(const std::string& new_nickname);
    void set_port(int new_port);
    void set_password(const std::string& new_password);
    void set_client_fd(int new_socket_fd);
    void set_server_address(sockaddr_in new_server_address);
    void set_realname(std::string new_realname);
    void set_ip(std::string new_ip);
    std::vector<std::string> get_channels() const;
    void add_channel(const std::string channel_name);
    void remove_channel(const std::string channel_name);

};
#endif