#include "server.hpp"



int	main(){
	try{
		int	socketId = socket(AF_INET, SOCK_STREAM, 0);
	if (socketId == -1){
		std::cerr << "Server not created!" << std::endl;
		return (1);
	}
	struct	sockaddr_in	sockaddress;
	memset(&sockaddress, 0, sizeof(sockaddress));
	sockaddress.sin_family = AF_INET;
	sockaddress.sin_port = htons(6667);
	sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(socketId, (struct sockaddr *)&sockaddress, sizeof(sockaddress)) == -1){
		std::cerr << "ERror at binding" << std::endl;
		close(socketId);
		return (1);
	}
	std::cout << "Server is binded to port : 6667" << std::endl; 
	if (listen (socketId, 200) == -1){
		std::cerr << "Not listening!" << std::endl;
		return (1);
	}
	std::cout << "Server is listening ..." << std::endl;
	while (1){
		struct sockaddr_in	connectedClientAddr;
		memset(&connectedClientAddr, 0, sizeof(connectedClientAddr));
		socklen_t	ClientAdrLength = 0;
		int	connectionServerId = accept(socketId, (struct sockaddr *) &connectedClientAddr, &ClientAdrLength);
		if (connectionServerId == -1){
			close (socketId);
			throw ("Failed to accept connection with client!");
		}
		std::cout << "accept a request at socket ID : " << connectionServerId << std::endl;
		char	receivedMsg[1024];
		memset(receivedMsg, 0, sizeof(receivedMsg));
		recv(connectionServerId, receivedMsg, 1024, 0);
		std::cout << "Server received : " << receivedMsg << std::endl;
		close (connectionServerId);
	}
		close (socketId);
	}
	catch(...){

	}

	
}