#include <iostream>
#include "CommandHandler.h"

int main(){
	std::string cmd;
	ch.showWelcome();
	ch.showPrompt();
	std::getline(std::cin,cmd);
	while(ch.execute(cmd) > 0){
		ch.showPrompt();
		std::getline(std::cin,cmd);
	}
	return 0;	
}