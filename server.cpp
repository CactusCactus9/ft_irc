#include "Server.hpp"

// bool	signal = false;

Server::Server(){serverID = -1;}

void	Server::setPort(int n){
	port = n;
}
// void	sigHandler(int signum){
// 	(void)signum;
// 	signal = true;//to stop the server
// }
void	Server::clearClients(int fd){
	for (size_t i = 0; i < fds.size(); ++i){//remove client from fds vector
		if(fds[i].fd == fd){
			fds.erase(fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < Clients.size(); ++i){//remove client from Clients vector
		if(Clients[i].getClientID() == fd){
			Clients.erase(Clients.begin() + i);
			break ;
		}
	}
}void	Server::closeFD(){
	for (size_t i = 0; i < Clients.size(); ++i)
		close(Clients[i].getClientID());
	if (serverID == -1)
		close(serverID);
}

void		Server::create_socket(){
	struct sockaddr_in serveraddress;
	struct pollfd		pollf;		
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(this->port);
	serveraddress.sin_addr.s_addr = INADDR_ANY;

	serverID = socket(AF_INET, SOCK_STREAM, 0);
	if (serverID == -1)
		throw (std::runtime_error("Server failed to get created!"));
	int	val = 1;
	if (setsockopt(serverID, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
		throw (std::runtime_error("the reuse of the address has failed!"));
	if (fcntl(serverID, F_SETFL, O_NONBLOCK) == -1)
		throw ("Failed to set nonblocking flag!");
	if (bind(serverID, (const sockaddr *)&serveraddress, sizeof(serveraddress)) == -1)
		throw(std::runtime_error("Binding to IP and port failed!"));
	if (listen(serverID, SOMAXCONN) == -1)
		throw (std::runtime_error("server isn't listening!"));
	pollf.fd = serverID;
	pollf.events = POLLIN;//flag to indicate theres data to read
	pollf.revents = 0;
	fds.push_back(pollf);
	std::cout << "server is listening from port : " << this->port << std::endl;
}


// // int	server_init(){
// // 	int	serverID = socket (AF_INET, SOCK_STREAM, 0);
// // 	if (serverID == -1){
// // 		std::cerr << "Server failer to get created!" << std::endl;
// // 		return (1);
// // 	}
// // 	return (serverID);
// // }

// struct sockaddr_in	init_struct(int n){
// 	struct sockaddr_in	sockaddress;//socket address
// 	memset(&sockaddress, 0, sizeof(sockaddress));
// 	sockaddress.sin_family = AF_INET;
// 	sockaddress.sin_port = htons(n);
// 	sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
// 	return (sockaddress);
// }

// int	main(int ac, char **av){
// 	if (ac != 3){
// 		std::cerr << "Enter a port and a password!" << std::endl;
// 		return (1);
// 	}
// 	//----Create server----//
// 	int	serverID = socket (AF_INET, SOCK_STREAM, 0);
// 	if (serverID == -1){
// 		std::cerr << "Server failer to get created!" << std::endl;
// 		return (1);
// 	}
// 	struct sockaddr_in	serveraddres = init_struct(strtol(av[1], NULL, 10));//struct sockaddr : struct used by kernel to store addresses
// 	if (bind(serverID, (struct sockaddr *)&serveraddres, sizeof(serveraddres)) == -1){
// 		std::cerr << "Server not binded!" << std::endl;
// 		close(serverID);
// 		return (1);
// 	}
// 	if (listen (serverID, 200) == -1){
// 		std::cerr << "Server isn't listening!" << std::endl;
// 		return (1);
// 	}
// 	while (1){
// 		struct sockaddr_in	clientaddress;
// 		memset(&clientaddress, 0, sizeof(clientaddress));
// 		socklen_t			clientaddrlen = 0;
// 		int	connectionID = accept(serverID, (struct sockaddr *)&clientaddress, &clientaddrlen);
// 		if (connectionID == -1){
// 			close(serverID);
// 			std::cerr << "Failed to connect!" << std::endl;
// 			return (1);
// 		}
// 		char	buffer[1024];
// 		memset(buffer, 0, sizeof(buffer));
// 		recv()
// 	}
// }