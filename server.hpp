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
#include <exception>


class	Server{
	private:
		int							serverID;
		int							port;
		static bool					signal;
		std::vector<Client>			Clients;
		std::vector<struct pollfd>	fds;
	public:
		Server();
		void		setPort(int n);
		void		create_socket();
		void		acceptClient();
		void		recieve_data(int fd);
		static void	sigHandler(int signum);
		void		closeFD();
		void		clearClients(int fd);
};

#endif