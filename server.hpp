


#ifndef SERVER_HPP
#define SERVER_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "Client.hpp"
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <csignal>
#include <cstring>
#include <string>
#include <cstdlib>
#include <map>

//TODO : need canonical form for classes
class	Server{
	private:
		int									serverID;
		int									port;
		std::string							password;
		std::string							nick;
		std::string							user;
		static bool							signal;
		std::vector<Client>					Clients;
		std::vector<struct pollfd>			fds;
		int									connectionID;
		std::string 						command;
		std::string 						args;
		std::map<std::string, std::string>	map;
	public:
		Server();
		//--Setters--//
		void		setPort(int n);
		void		setPassword(char *str);
		//--Getters--//
		int			getPort();
		std::string	getPassword();
		//----//
		void		create_socket();
		void		launch_server();
		void		multi_clients();
		void		acceptClient();
		void		recieve_data(int fd);
		static void	sigHandler(int signum);
		void		closeFD();
		void		clearClient(int fd);
};

#endif