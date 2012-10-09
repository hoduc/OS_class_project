#include <string>
#include "macro.h"

//structure holding a process
typedef struct process{
	std::string _name;
	CLASS _class;
	STATE _state;
	PRIORITY _priority;
	int _mem;
}PROCESS;
typedef process* pPROCESS;	//pointer to process

//a queuenode structure 
typedef struct queueNode{
	PROCESS _process;
	struct queueNode* _prev;
	struct queueNode* _next; 
	queueNode():_prev(NULL), _next(NULL){}
}QNODE;
typedef struct queueNode* pQUEUE;	//pointer to queuenode

//queuelist class
typedef class qList{
private:
	pQUEUE head;
	pQUEUE tail;
	int sz;
public:
	qList():sz(0){
		head = tail = new QNODE;
	}
	
	//adding a process to the queue
	void enQueue(pQUEUE &aProcess){
		if (empty()){
			head = aProcess;
			tail = head;
		}
		else{
			pQUEUE temp = aProcess;
			temp->_prev = tail;
			tail->_next = temp;
			tail = temp;
		}
		sz++;
	}
	
	//removing a process from queue
	pQUEUE dequeue(pQUEUE &aProcess){
		if (aProcess == head){
			head = aProcess->_next;
			//aProcess = NULL;
		}
		
		else if (aProcess == tail){
			tail = aProcess->_prev;
			//aProcess = NULL;
		}
		
		else{
			aProcess->_prev->_next = aProcess->_next;
			aProcess->_next->_prev = aProcess->_prev;
		}
		sz--;
		return aProcess;
	}
	
	//find a process
	pQUEUE findProcess(std::string name){
		if (empty())
			return NULL;
		pQUEUE t = head;
		while (t != tail->_next){
			if (name == (t->_process._name))
				return t;
			t= t->_next;
		}
		return NULL;
	}

	void printQueue(){
		pQUEUE t = head;
		std::cout << sz << std::endl;
		if (empty()){
			std::cout << std::endl;
			return;
		}
		while (t != tail->_next){
			std::cout << "\"" << t->_process._name << "\"" << "[";
			switch(t->_process._state){
				case 	READY:	std::cout << "\"" << "re";
									break;
				case BLOCKED:  std::cout << "\"" << "bl";
									break;
				case SUSPENDED_READY:	std::cout << "\"" << "sre";
												break;
				case SUSPENDED_BLOCKED:	std::cout << "\"" << "sbl";
												break;
				case RUNNING:				std::cout << "\"" << "ru";
												break;
				case SUSPENDED:			std::cout << "\"" << "su";
												break;
			}
			std::cout << "," << (t->_process._priority) << "]";
			//std::cout << t->_process._class 
			std::cout << std::endl;
			t = t->_next;
		}

	}

	bool empty(){
		return sz == 0;
	}
}QUEUE;
typedef qList* QT;

//Queue list using 4 queue
class PCB{
private:
	QT _qt;
	int sz;
public:
	PCB():sz(4){  
		_qt = new QUEUE[sz];
	}

	pQUEUE allocatePCB(){
		return new QNODE;
	}

	pQUEUE setUpPCB(std::string process_name, int priority, int process_type){
		//test for priority and type
		if ((priority >= -128 && priority <= 127)
		&& (process_type >=0 && process_type <= 1)
		&& (findPCB(process_name)== NULL)){
			pQUEUE pp = allocatePCB();
			pp->_process._name = process_name;
			pp->_process._priority = (PRIORITY)priority;
			pp->_process._class = (CLASS)process_type;
			pp->_process._state = READY;
			pp->_process._mem = 1;
			return pp;
		}
		else{
			std::cout << "Something wrong!!!" << std::endl;
			return NULL;
		}
	}

	pQUEUE findPCB(std::string pName){
		//iterating through 4 queues
		//to find the process
		for (int i = 0; i <  sz; ++i){
			pQUEUE p = _qt[i].findProcess(pName);
			if (p != NULL)
				return p;
		}
		return NULL;
	}

	void insertPCB(int queue, pQUEUE &pi){
		_qt[(STATE)queue].enQueue(pi);
	}

	pQUEUE removePCB(pQUEUE &p){
		return _qt[(STATE)p->_process._state].dequeue(p);
	}

	void freePCB(pQUEUE &p){
		delete p;
	}

	void printQueue(){
		for (int i = 0; i < sz; ++i){
			switch(STATE(i)){
				case READY		: std::cout << "Ready Queue : ";
									  break;
				case SUSPENDED_READY	: std::cout << "Suspended_Ready Queue : ";
											  break;
				case BLOCKED	: std::cout << "Blocked Queue : ";
									  break;
				case SUSPENDED_BLOCKED : std::cout << "Suspended_Blocked Queue : ";
												 break;
				
			}
			_qt[i].printQueue();
		}
	}


};

PCB pcb;