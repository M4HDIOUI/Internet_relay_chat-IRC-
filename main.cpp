/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omahdiou <omahdiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 04:47:50 by omahdiou          #+#    #+#             */
/*   Updated: 2024/08/28 04:48:02 by omahdiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout<< "bad arguments" << std::endl;
        exit(EXIT_FAILURE);   
    }
    try
    {
        Server Server(av[1], av[2]);
    }
    catch (const std::exception &excp)
    {
        std::cout<< excp.what() << std::endl;
    }
}
