#include "macro.h"
#include "PCB.h"
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
	al.insert(std::make_pair("cp", "cp"));
	al.insert(std::make_pair("dp", "dp"));
	al.insert(std::make_pair("block", "block"));
	al.insert(std::make_pair("ublock","ublock"));
	al.insert(std::make_pair("suspend", "suspend"));
	al.insert(std::make_pair("resume", "resume"));
	al.insert(std::make_pair("sp", "sp"));
	al.insert(std::make_pair("showp","showp"));
	al.insert(std::make_pair("showa", "showa"));
	al.insert(std::make_pair("showr", "showr"));
	al.insert(std::make_pair("showb", "showb"));

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
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (src.empty() && dest.empty()){
			//let user enter information
			std::string process_name;
			int priority;
			int process_type;
			std::cout << "Enter process name : ";
			std::cin  >> process_name;
			std::cout << "Enter priority[-127->128]: ";
			std::cin >> priority;
			std::cout << "Enter process type[0-APP, 1-SYS]: ";
			std::cin  >> process_type;
			
			pQUEUE pp = pcb.setUpPCB(process_name, priority, process_type);
			if (pp!= NULL)
				pcb.insertPCB(READY, pp);
		}
		else std::cout << cmd << "take no parameter" <<std::endl;
		return OK;
	}
};

class DeletePCB : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				pcb.removePCB(p);
				pcb.freePCB(p);
			}
			else std::cout << "No such process" << std::endl;

		}
		else std::cout << cmd << "take 1 parameter only" << std::endl;
		return OK;
	}
};

class Block : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				//if the process is not blocked
				if (p->_process._state == READY){
					pcb.removePCB(p);
					pcb.insertPCB(BLOCKED, p);
					p->_process._state = BLOCKED;
				}
				else std::cout << "Process " << p->_process._name << " is already blocked" << std::endl;
			}
			else std::cout << "No such process" << std::endl;
		}
		else std::cout << cmd << " take 1 parameter" << std::endl;
		return OK;
	}
};

class UBlock : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				//if the process is blocked --> unblock
				if (p->_process._state == BLOCKED){
					pcb.removePCB(p);
					pcb.insertPCB(READY, p);
					p->_process._state = READY;
				}
				else std::cout << "Process :" << p->_process._name << "is already unblocked" << std::endl;
			}
			else std::cout << "No such process" << std::endl;
		}
		else std::cout << cmd << " take 1 parameter" << std::endl;
		return OK;
	}	
};

class Suspend : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				if (p->_process._state != SUSPENDED){
					pcb.removePCB(p);
					pcb.insertPCB(SUSPENDED_READY, p);
					p->_process._state = SUSPENDED;
				}
				else std::cout << "Process :" << p->_process._name << "is already suspended" << std::endl;
			}
			else std::cout << "No such process" << std::endl;
		}
		else std::cout << cmd << " take 1 parameter" << std::endl;
		return OK;
	}	
};

class Resume : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				if (p->_process._state == SUSPENDED){
					pcb.removePCB(p);
					pcb.insertPCB(READY, p);
					p->_process._state = READY;
				}
				else std::cout << "Process :" << p->_process._name << "is already resumed" << std::endl;
			}
			else std::cout << "No such process" << std::endl;
		}
		else std::cout << cmd << " take 1 parameter" << std::endl;
		return OK;
	}	
};

class SetPriority : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (!src.empty() && !dest.empty()){
			pQUEUE p = pcb.findPCB(src);
			if (p != NULL){
				int pri = atoi(dest.c_str());
				if (pri >= -127 && pri <= 128){
					p->_process._priority = pri;
				}
				else std::cout << "Priority must be w/in range [-128,127]" << std::endl;
			}

			else std::cout << "No such process exist" << std::endl;
			return OK;
		}
	}
};

class ShowPCB : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
		if (src.empty() && dest.empty()){
			std::string name;
			std::cout << "Enter process name: ";
			std::cin >> name;
			pQUEUE p = pcb.findPCB(name);
			if (p != NULL){
				std::cout << "Class\t: " << p->_process._class << std::endl;
				std::cout << "Memory\t: " << p->_process._mem << std::endl;
				std::cout << "Name\t:" << p->_process._name << std::endl;
				std::cout << "Priority\t:" << p->_process._priority << std::endl;
				std::cout << "State\t:" << p->_process._state << std::endl;
			}

			else std::cout << "No such process exist" << std::endl;
				
		}
		else std::cout << cmd << " take no parameter" << std::endl;
		std::cin.ignore(INT_MAX,'\n');
		return OK;
	}
};

class ShowAll : public Command{
public:
	int exe(const std::string &cmd,
			  const std::string &src,
			  const std::string &dest){
	if (src.empty() && dest.empty()){
		pcb.printQueue();
	}
	else std::cout << cmd << " take no parameter" << std::endl;
	return OK;
	}

};

Command *help = new Help();
Command *version = new Version();
Command *date = new Date();
Command *quit = new Exit();
Command *history = new History();
Command *prompt = new Prompt();
Command *alias = new Alias();
Command *cp = new CreatePCB();
Command *dp = new DeletePCB();
Command *block = new Block();
Command *ublock = new UBlock();
Command *suspend = new Suspend();
Command *resume = new Resume();
Command *sp = new SetPriority();
Command *showp = new ShowPCB();
Command *showa = new ShowAll();