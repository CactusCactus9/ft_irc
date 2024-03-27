#include "Server.hpp"

bool	Server::signal = false;

void	Server::sigHandler(int signum){
	(void)signum;
	std::cout << "signal found!" << std::endl;
	signal = true;//to stop the server
}
Server::Server(){serverID = -1;}

void	Server::setPort(int n){
	port = n;
}

void	Server::setPassword(char *str){
	password = str;
}

void	Server::clearClient(int fd){
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
	for (size_t i = 0; i < Clients.size(); ++i){//close clients fd
		std::cout << "client disconnected" << std::endl;
		close(Clients[i].getClientID());}
	if (serverID == -1){//close server socket
		std::cout << "server disconnected" << std::endl;
		close(serverID);}
}

void		Server::create_socket(){
	struct sockaddr_in serveraddress;
	struct pollfd		pollf;		
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(this->port);
	serveraddress.sin_addr.s_addr = INADDR_ANY;//any local machine address

	serverID = socket(AF_INET, SOCK_STREAM, 0);
	if (serverID == -1)
		throw (std::runtime_error("Server failed to get created!"));
	// std::cout << "Socket created successfully" << std::endl;
	int	val = 1;
	if (setsockopt(serverID, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)//SET OPTIONS OF SOCKET: SOL_SOCKET:the option is defined at socket level/ SO_REUSADDR : the option that allows to reuse local addresses which can be useful in scenarios where you want to quickly restart a server on the same address and port after it has been stopped.
		throw (std::runtime_error("the reuse of the address has failed!"));
	// std::cout << "Setsocketopt successfully" << std::endl;
	if (fcntl(serverID, F_SETFL, O_NONBLOCK) == -1)//PERFORM OPERATIONS ON FD : F_SETFL: THE OPERATION IS TO SET FILE STATUS FLAGS/ O_NONBLOCK : SOCKET NONBLOCKING
		throw ("Failed to set nonblocking flag!");
	std::cout << "fcntl successfully" << std::endl;
	if (bind(serverID, (const sockaddr *)&serveraddress, sizeof(serveraddress)) == -1)//bind the server to a port and IP
		throw(std::runtime_error("Binding to IP and port failed!"));
	// std::cout << "Socket binded" << std::endl;
	if (listen(serverID, SOMAXCONN) == -1)//socket is passive and listening to coming connections
		throw (std::runtime_error("server isn't listening!"));
	std::cout << "Socket listening ..." << std::endl;
	pollf.fd = serverID;//initialize the fds with server
	pollf.events = POLLIN;//flag to indicate theres data to read
	fds.push_back(pollf);//initialize fds vector
	std::cout << "server is listening from port : " << this->port << std::endl;
}
void	Server::launch_server(){
	create_socket();
	multi_clients();
}

void	Server::acceptClient(){
	Client				client;
	struct sockaddr_in	clientaddress;
	struct pollfd		newpool;
	socklen_t			clientaddrlen = 0;

	memset(&clientaddress, 0, sizeof(clientaddress));
	int	connectionID = accept(serverID, (struct sockaddr *)&clientaddress, &clientaddrlen);//new socket to 	assure safe communication with multiple clients 
	if (connectionID == -1){
		std::cerr << "Failed to connect!" << std::endl;
		return ;
	}
	if (send(connectionID, "enter: password, nickname, and username\n", 40, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
	if (fcntl(connectionID, F_SETFL, O_NONBLOCK) == -1){//server configuration can impact client's so we set new socket as server 
		std::cerr << "failed to set nonblocking option!" << std::endl;
		return ;
	}
	newpool.fd = connectionID;
	newpool.events = POLLIN;

	client.setClientID(connectionID);
	client.setIP(inet_ntoa(clientaddress.sin_addr));
	Clients.push_back(client);
	fds.push_back(newpool);
	std::cout << "accepted!" << std::endl;
}

void	Server::recieve_data(int fd){
	char	buffer[1024];
	Client	c;

	memset(buffer, 0, sizeof(buffer));
	size_t	total = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (total <= 0){
		std::cout << "client gone" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else
		this->Clients[fd].setBuffer(buffer);
	// if (this->Clients[fd].setBuffer.find_first_of("\n\r") == std::string::npos)
	// 	return;
	password += "\n";
	std::cout << buffer << std::endl;
	if (strcmp(buffer, this->password.c_str()))
		std::cout << "word" << std::endl;


}

void	Server::multi_clients(){
	while (Server::signal == false){
		if (poll(&fds[0], fds.size(), -1) == -1 && Server::signal == false)//poll blocked indefinitely till an event occurs or ctrl c
			throw (std::runtime_error("poll() failed!"));
		for (size_t i = 0; i < fds.size(); ++i){
			if (fds[i].revents & POLLIN)//returned event: data to read
			{
				if (fds[i].fd == serverID)
					acceptClient();
				else
					recieve_data(fds[i].fd);
			}
		}
	}
	closeFD();
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