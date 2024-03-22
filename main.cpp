#include "Server.hpp"
#include "Client.hpp"

int	main(int ac, char **av){
	try{
		if (ac != 3){
			std::cerr << "Enter a port and a password" << std::endl;
			return (1);
		}
		Server	serv;
		serv.setPort(strtol(av[1], NULL, 10));
		// signal(SIGINT, Server::sigHandler());
		serv.create_socket();
	}
	catch(const std::exception &e){
		std::cout << e.what() << std::endl;
	}
}