#include "Server.hpp"

int	main(int ac, char **av){
	Server	serv;
	try{
		int			por;
		std::string	pas;
	
		if (ac != 3)
			throw(std::runtime_error("Enter a port and a password"));
		std::string	password = av[2];
		if (password.find_first_of(" \t") != std::string::npos)
			throw(std::runtime_error("NO WHITE SPACES ALLOWED!"));
		serv.setPort(strtol(av[1], NULL, 10));
		serv.setPassword(av[2]);
		por = serv.getPort();
		std::string	s = av[1];
		if (s.find_first_not_of("0123456789") != std::string::npos){
			throw(std::runtime_error("Only numbers in port!"));
		}
		pas = serv.getPassword();
		if (por < 1024 || por > 49151){
			throw(std::runtime_error("Enter a valid port"));
		}
		if (pas.size() <= 0){
			throw(std::runtime_error("Empty Password!"));
		}
		signal(SIGINT, Server::sigHandler);
		signal(SIGQUIT, Server::sigHandler);
		serv.launch_server();
	}
	catch(const std::exception &e){
		serv.closeFD();
		std::cerr << e.what() << std::endl;
	}
}
