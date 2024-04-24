/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:17:48 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/16 16:48:25 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include <cstdlib>

int	main(int ac, char **av){
	Server	serv;
	try{
		int			por;
		std::string	pas;
	
		if (ac != 3){
			std::cerr << "Enter a port and a password" << std::endl;
			return (1);
		}
		serv.setPort(strtol(av[1], NULL, 10));
		serv.setPassword(av[2]);//MODIFIED
		por = serv.getPort();
		std::string	s = av[1];
		if (s.find_first_not_of("0123456789") != std::string::npos){
			throw(std::runtime_error("Only numbers in port!"));//MODIFIED
		}

		pas = serv.getPassword();
		if (por < 1024 || por > 49151)
			throw(std::runtime_error("Enter a valid port"));//MODIFIED
		if (pas.size() <= 0)
			throw(std::runtime_error("Empty Password!"));//MODIFIED
		signal(SIGINT, Server::sigHandler);
		signal(SIGQUIT, Server::sigHandler);
		serv.launch_server();
	}
	catch(const std::exception &e){
		serv.closeFD();
		std::cout << e.what() << std::endl;
	}
}
