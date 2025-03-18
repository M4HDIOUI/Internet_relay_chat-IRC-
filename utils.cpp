/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:50:37 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 04:50:44 by omahdiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> split_input(std::string str)
{
    std::vector<std::string> channels;
    std::string temp;
    std::istringstream stream(str);


    while (stream >> temp) 
    {
        std::string::size_type start = 0;
        std::string::size_type pos = temp.find(',');

        while (pos != std::string::npos) 
        {
            channels.push_back(temp.substr(start, pos - start));
            start = pos + 1;
            pos = temp.find(',', start);
        }

        if (start < temp.size()) 
        {
            channels.push_back(temp.substr(start));
        }
    }
    return channels;
}

bool check_spaces(std::string str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\r')
        {
            return (false);
        }
    }
    return (true);
}