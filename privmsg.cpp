#include "Server.hpp"

int	Server::validArgsPriv(std::string &args){
	size_t		                count = 0;
	size_t		                ind;

	if (args == "\0"){//FIXED
		return (0);
	}
	
	size_t	index = args.find_first_of(" \t\r','");
	if (index == std::string::npos || (args[0] == ':')){
		return (2);
	}
	//*****IN CASE OF MANY CLIENTS/CHANNELS
	//------count commas------//
	if (args[index] == ','){
		for (size_t i = 0; i < args.size(); i++){
			if (args[i] == ',')
				count++;
		}
		std::cout << "count of commas:" << count << std::endl;
		size_t	start = 0;
		ind = index;
		//------fill vector with clients/channels------//
		for (size_t i = 0; i <= count; i++){
			this->vec.push_back(args.substr(start, ind - start));
			std::cout << "i=" << i << "---" << "vec[i]:" << vec[i] << "----0" << std::endl;
			start = ind + 1;
			ind = args.find_first_of("','", start);
			if (ind == std::string::npos)
				ind = args.find_first_of(" \t\r");	
		}
	}
	size_t i = index;
	size_t j;
	
	size_t ikram = (args.find_last_of(" \t\r"));
	if (args[index] == ','){
		i = ind;
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++);//IN CASE OF CLIENT1, CLIENT2  ?!
		// //------compare vec with clients------//
		if (args[index] == ','){
			for (size_t M = 0; M < vec.size(); ++M){
				for(j = 0; j < clients.size(); ++j){
					if (vec[M] == clients[j].getNickname()){
						this->message = (args.substr(ikram + 1, args.size()));
						std::cout << "where message should end:" << ikram << "----+*" << args[ikram] << "====="<< std::endl;
						std::cout << "i" << i << "---" << args[i] << "----message to many clients:" << this->message << "-----/" << std::endl; 
						sendMsg(clients[j].getClientFD(), this->message);
						sendMsg(clients[j].getClientFD(), "\n");
						break ;
					}
				}
					if (j == clients.size())
						sendMsg(clients[M].getClientFD(), ERR_NOSUCHNICK(clients[j].getNickname(), vec[i]));
			}

		}
	}

	// }
	else{
						
	//------skip white spaces------//
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++);
		this->target = args.substr(0, index);
		this->message = (args.substr(ikram + 1, args.size()));//TODO: NEED TO GET ONLY FIRST PART
		std::cout << "target:" << this->target << "------+++++" << std::endl;
		std::cout << "message in case of one element:" << this->message << "------+++++" << std::endl;
		size_t m;
		for(m = 0; m < clients.size(); m++){
			if (this->target == clients[m].getNickname())
				return (1);
		}
			puts("aji hna");
		if (m == clients.size())
			return (3);

		
	}
	std::cout << "this->target:" << this->target << "---;" << std::endl;
		std::cout << "this->message:" << this->message << "---;" << std::endl;
	return (1);
}

void	Server::privmsgCommand(std::string &args, Client &cli){
    (void)args;
    if (!cli.isRegistered()){
        sendMsg(cli.getClientFD(), ERR_NOTREGISTERED(cli.getNickname()));
        return ;
    }
	if (validArgsPriv(args) == 1){
			// sendMsg(cli.getClientFD(), this->message);
			sendMsg(cli.getClientFD(), "\n");
			}
	else if(validArgsPriv(args) == 0){
		sendMsg(cli.getClientFD(), ERR_NO_RECIPIENT(cli.getNickname()));
	}
	else if(validArgsPriv(args) == 2){
		sendMsg(cli.getClientFD(), ERR_NO_TEXT(cli.getNickname()));
	}
	else if(validArgsPriv(args) == 3){
		sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(cli.getNickname(), this->target));
	}
	vec.clear();
	// else if(validArgsPriv(args) == 8){
	// 	sendMsg(cli.getClientFD(), "\n");
	// }
}