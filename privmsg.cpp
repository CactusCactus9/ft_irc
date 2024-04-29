#include "Server.hpp"
int	ft_count(std::string str){
	size_t	count = 0;
	for (size_t i = 0; i < str.size(); i++){
			if (str[i] == ',')
				count++;
		}
		return (count);
}

void	Server::store_clients_channels(std::string &args, size_t count, size_t ind, size_t start){
	for (size_t i = 0; i <= count; i++){
			if (args[start] == '#'){
				if (ind - start == 0)
					break ;
				this->vec_ch.push_back(args.substr(start, ind - start));
			}
			else{
				if (ind - start == 0)
					break ;
				this->vec_cl.push_back(args.substr(start, ind - start));

			}
			start = ind + 1;
			ind = args.find_first_of("','", start);
			if (ind == std::string::npos)
				ind = args.find_first_of(" \t\r");	
				if (ind == std::string::npos)
					std::cout << "failed to find ind" << std::endl;
		std::cout << "args[ind]" << &args[ind] << "****" << std::endl;
		}
}

void	Server::sendToClients(size_t msg_begin, Client &cli, bool isMessage){
	size_t	M = 0;
	
	for (; M < vec_cl.size(); ++M){
		if ((isInUseNickname(vec_cl[M]) == true)){
			if (isMessage == false)
				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			sendMsg(findClient(vec_cl[M]).getClientFD(), this->message);
			sendMsg(findClient(vec_cl[M]).getClientFD(), "\n");
		}
		else
			sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(vec_cl[M], this->target));
	}
}

void	Server::sendToChannels(size_t msg_begin, Client &cli, bool isMessage){
	for (size_t M = 0; M < vec_ch.size(); ++M){
		if ((isInUseChName(vec_ch[M]) == true)){
			if (isMessage == false)
				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			Channel	chan = findChannel(vec_ch[M]);
			chan.sendmsg2chanOperators(*this, this->message);
			chan.sendmsg2chanRegulars(*this, this->message);
		}
		else
			sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
	}
}

int	Server::validArgsPriv(std::string &args, Client &cli){
	size_t	count = 0;
	size_t	ind = 0;
	size_t	start = 0;
	bool	isMessage = false;

	size_t	index = args.find_first_of(" \t\r','");
	if (index == std::string::npos || (args[0] == ':')){
		return (2);
	}
	if (args[index] == ','){
		count = ft_count(args);
		ind = index;
		store_clients_channels(args, count, ind, start);
	}
	size_t i = index;
	size_t msg_begin = (args.find_last_of(" \t\r"));
	if (args[index] == ','){
		i = ind;
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++) 
		if (args[i] == ':'){
			this->message = (args.substr(i , args.size()));
			isMessage = true;
		}
		sendToClients(msg_begin, cli, isMessage);
		sendToChannels(msg_begin, cli, isMessage);
	}

	else{
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++)
		std::cout << "args[i],,,,,,,,,,,,," << args[i] << std::endl; 
		if (args[i] == ':'){
			this->message = (args.substr(i + 1 , args.size()));
			isMessage = true;
		}
		this->target = args.substr(0, index);
		if (isMessage == false)
			this->message = (args.substr(msg_begin + 1, args.size()));//TODO: NEED TO GET ONLY FIRST PART//max 150 characters?
		std::cout << "target[0]:" << this->target[0] << "------+++++" << std::endl;
		std::cout << "message in case of one element:" << this->message << "------+++++" << std::endl;
		// size_t m;
		// for(m = 0; m < clients.size(); m++){
		if (this->target[0] == '#'){
			std::cout << ":return dial isInUseChName(this->target)" << isInUseChName(this->target) << "&&&&&&&//////" << std::endl;
			if (isInUseChName(this->target) == true){
				Channel	chan = findChannel(this->target);
				std::cout << "chan" << chan.getName() << "----------*" << std::endl;
				chan.sendmsg2chanOperators(*this, this->message);
				puts("truuuuuuuuuuuue");
				chan.sendmsg2chanRegulars(*this, this->message);
				return (1);
			}
			else
				sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
		}
		else if (this->target[0] != '#'){
			if (isInUseNickname(this->target) == true){
				puts("found target");
				sendMsg(cli.getClientFD(), this->message);sendMsg(cli.getClientFD(), "\n");
				return (1);
			}
		}
		else
			return (3);

		
	}
	std::cout << "this->target:" << this->target << "---;" << std::endl;
		std::cout << "this->message:" << this->message << "---;" << std::endl;
	return (1);
}

void	Server::privmsgCommand(std::string &args, Client &cli){
    
	if (validArgsPriv(args, cli) == 1)
			;
	else if(validArgsPriv(args, cli) == 2){
		sendMsg(cli.getClientFD(), ERR_NO_TEXT(cli.getNickname()));
	}
	else if(validArgsPriv(args, cli) == 3){
		sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(cli.getNickname(), this->target));
	}
	
	
	// else if(validArgsPriv(args) == 8){
	// 	sendMsg(cli.getClientFD(), "\n");
	// }
}