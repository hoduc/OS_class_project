#include "macro.h"
#include <stack>
#include <string>
#include <fstream>
#include <map>
//cross-platform code
#if defined (__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#include "windows.h"
#define OS_WIN
#else
#include "sys/unistd.h"
#endif


static std::stack<std::string> uc;		//used command
static std::map<std::string,std::string> al;	//alias list

void init_alias(){
	al.insert(std::make_pair("help","help"));
	al.insert(std::make_pair("version","version"));
	al.insert(std::make_pair("date","date"));
	al.insert(std::make_pair("exit","exit"));
	al.insert(std::make_pair("history","history"));
	al.insert(std::make_pair("prompt","prompt"));
	al.insert(std::make_pair("alias","alias"));
}
void commandToStack(const std::string &cmd){
		//find the command on stack
		std::stack<std::string> t = uc;
		while (!t.empty()){
			if (t.top() == cmd)
				return;
			t.pop();
		}
		//no such command existed on the stack
		uc.push(cmd);
	}

class Command{
public:
	virtual ~Command(){};
	virtual int exe(const std::string &cmd,
						 const std::string &src,
						 const std::string &dest)=0;
};

class Help : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
		     const std::string &dest){
	   std::fstream hdoc;				//the help documentation
		std::string help;
	   //help takes no parameter
		if (src.empty() && dest.empty()){
			//extract the info out of text file
			hdoc.open("help.txt", std::ios::in);
			hdoc.seekg(0,std::ios::beg);
			while (getline(hdoc, help) != NULL){
				std::cout << help << std::endl;
			}
			hdoc.close();
			//add to stack
			commandToStack(cmd);
		}
		else std::cout << cmd << "take no parameter" << std::endl;
		return OK;
	}
};



class Version : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		std::fstream vdoc;			//version doc
	   std::string version;
		if (src.empty() && dest.empty()){
			vdoc.open("version.txt", std::ios::in);
			vdoc.seekg(0, std::ios::beg);
			getline(vdoc,version);
			if (!version.empty()){
				std::cout << version << std::endl;
			}
			vdoc.close();
			//add to stack
			commandToStack(cmd);
		}
		else std::cout << cmd << " take no parameter" << std::endl;
		return OK;
	}
};

class Date : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		std::fstream ddoc;				//date documentation
		std::string date;

		//showing date
		if (src.empty() && dest.empty()){
			ddoc.open("date.txt", std::ios::in);
			ddoc.seekg(0, std::ios::end);
			int len = ddoc.tellg();
			ddoc.close();
			//empty file
			if (len == 0){
				//write out current date
				#if defined OS_WIN
					SYSTEMTIME dat;
					GetSystemTime(&dat);
					ddoc.open("date.txt", std::ios::out);
					ddoc.seekg(0,std::ios::beg);
					ddoc << dat.wMonth << "-" << dat.wDay << "-" << dat.wYear;
					ddoc.close();

					ddoc.open("date.txt", std::ios::in);
					getline(ddoc,date);
					std::cout << date;
					ddoc.close();
				#endif
			}
			
			//a date was set
			else{
				ddoc.open("date.txt", std::ios::in);
				ddoc.seekg(0, std::ios::beg);
				getline(ddoc,date);
				std::cout << date << std::endl;
				ddoc.close();
			}
		}

		//set date
		else if (!src.empty() && dest.empty()){
			ddoc.open("date.txt", std::ios::out);
			ddoc.seekg(0, std::ios::out);
			ddoc << src;
			ddoc.close();
		}
		else std::cout << cmd << "take 1 or no argument";
		commandToStack(cmd);
		return OK;
	}
	
	~Date(){
		//clear the date
		std::fstream ddoc;
		ddoc.open("date.txt", std::ios::out);
		ddoc.clear();
	}
};

class Exit : public Command{
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		std::cout << "Do you want to exit (Y/N)?";
		char c;
		std::cin >> c;
		if (c == 'Y' || c == 'y'){
			#if defined OS_WIN
				for (int i = 0; i < 3; ++i){
					std::cout << ".";
					Sleep(1000);	
				}
				std::cout << std::endl;
			#endif
			return NOT_OK;
		}
		else{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			commandToStack(cmd);
			return OK;
		}
	}
};

class History : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		//showing the number of history
	   std::stack<std::string> t = uc;
		std::cout << "[";
		while (!t.empty()){
			std::cout << t.top() << ",";
			t.pop();
		}
		std::cout << "]" << std::endl;
		commandToStack(cmd);
		return OK;
	}
};

class Prompt : public Command{
private:
	std::string prompt;
public:
	Prompt():prompt(">"){};
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (src.empty() && dest.empty()){
			std::cout << prompt;
		}

		else if (!src.empty() && dest.empty()){
			commandToStack(cmd);
			prompt = src;
		}
		
		else std::cout << cmd << " only take one parameter" << std::endl;

		return OK;
	}
};

class Alias : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && !dest.empty()){
			commandToStack(cmd);
			//making sure there is such a command
			std::map<std::string, std::string>::iterator it;
			it = al.begin();
			while (it != al.end()){
				if (it->second.find(src) != std::string::npos){
					it->second += dest;
					return OK;
				}
				it++;
			}
			
			std::cout << "No such command" << std::endl;
		}
		
		else std::cout << cmd << "take 2 parameters" << std::endl;
		return OK;
	}
};

class CreatePCB : public Command{
};

class AllocatePCB : public Command{

}


Command *help = new Help();
Command *version = new Version();
Command *date = new Date();
Command *quit = new Exit();
Command *history = new History();
Command *prompt = new Prompt();
Command *alias = new Alias();
