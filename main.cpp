#include <iostream>
#include "CommandHandler.h"

int main(){
	std::string cmd;
	ch.showWelcome();
	ch.showPrompt();
	getline(std::cin,cmd,'\n');
	while(ch.execute(cmd) > 0){
		ch.showPrompt();
		std::cin.ignore();					//Flush the inputstream
		getline(std::cin,cmd,'\n');
	}
	return 0;	
}