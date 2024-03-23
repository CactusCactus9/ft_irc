#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class	Client{
	private:
		int			clientID;
		std::string	clientIP;
	public:
		Client();
		int		getClientID();
		void	setClientID(int fd);
		void	setIP(std::string IPaddr);
		~Client();
};

#endif