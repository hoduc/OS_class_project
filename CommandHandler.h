#include "Command.h"
#include <map>
#include <sstream>

class CommandHandler{
private:
	std::map<std::string, Command*> cm;	//command to methods	
	std::string cmd;
	std::string src;
	std::string dest;
public:
	CommandHandler(){
		cm.insert(std::make_pair("help", help));
		cm.insert(std::make_pair("version", version));
		cm.insert(std::make_pair("date", date));
		cm.insert(std::make_pair("exit", quit));
		cm.insert(std::make_pair("history", history));
		cm.insert(std::make_pair("prompt", prompt));
		cm.insert(std::make_pair("alias", alias));
		cm.insert(std::make_pair("cp", cp));
		cm.insert(std::make_pair("dp",dp));
		cm.insert(std::make_pair("block",block));
		cm.insert(std::make_pair("ublock",ublock));
		cm.insert(std::make_pair("resume",resume));
		cm.insert(std::make_pair("suspend", suspend));
		cm.insert(std::make_pair("showp",showp));
		cm.insert(std::make_pair("showa",showa));
		
		cmd = src = dest = "";
		init_alias();
	}

	bool parseCommand(std::string line){
		cmd = src = dest = "";
		std::string token;
		std::stringstream ss;
		ss << line;
		int counter = 0;
		while (ss.good()){
			if (counter > 3){
				return false;
			}
			switch(counter){
			case 0:	ss >> cmd;
						break;
			case 1:	ss >> src;
						break;
			case 2:	ss >> dest;
						break;
			}
			counter++;
		}

		return true;
	}

	int execute(std::string line){
		if (parseCommand(line) == false){
			std::cout << "The maximum number of argc is 2";
			return OK;
		}

		//find in alias list
		std::map<std::string, std::string>::iterator it;
		for (it = al.begin(); it != al.end(); ++it){
			if (it->second.find(cmd) != std::string::npos){
				break;
			}
		}

		if (it != al.end()){
			return cm.find(it->first)->second->exe(cmd,src,dest);
		}

		else {
			std::cout << "No such Command" << std::endl;
			return OK;
		}
	}

	void showWelcome(){
		std::cout << "HELLO there, do what you need to do!!!\n"
					 << "Type help for info\n";
	}

	void showPrompt(){
		std::map<std::string, Command*>::iterator it;
		it = cm.find("prompt");
		it->second->exe("prompt","","");
	}

	~CommandHandler(){
		std::map<std::string, Command*>::iterator it;
		it = cm.begin();
		while (it != cm.end()){
			delete it->second;
			it++;
		}
	}
};

CommandHandler ch;