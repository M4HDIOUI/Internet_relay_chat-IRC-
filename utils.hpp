/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:50:46 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 04:50:54 by omahdiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_CPP
#define UTILS_CPP

#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include "client.hpp"
#include <map>
#include "server.hpp"

std::vector<std::string> split_input(std::string str);
bool check_spaces(std::string str);

#endif