typedef enum {APP, SYS} CLASS;
//state of a process
//also to figure out the queue in
typedef enum{
	READY, BLOCKED, SUSPENDED_READY, SUSPENDED_BLOCKED,
RUNNING, SUSPENDED
}STATE;
typedef char PRIORITY;


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
	struct queue* _prev;
	struct queue* _next; 
	queue():_prev(NULL), _next(NULL){}
}QNODE;
typedef struct queue* pQUEUE;	//pointer to queuenode

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
	
	void enQueue(std::string name, char priority, int cLASS){
		if (empty()){
			head->_process._name = name;
			head->_process._priority = (PRIORITY)priority;
			head->_process._class = (CLASS)cLASS;
			tail = head;
		}
		else{
			pQUEUE temp = new QNODE;
			temp->_process._name = name;
			temp->_process._priority = (PRIORITY)priority;
			temp->_process._class = (CLASS)cLASS;
			temp->_prev = tail;
			tail->_next = temp;
			tail = temp;
		}
		sz++;
	}

	void dequeue(){
	
	}
	
	void printQueue(){
		pQUEUE t = head;
		int counter = 1;
		while (t != tail->_next){
			std::cout << counter++ << ":" << std::endl;
			std::cout << t->_process._name << std::endl;
			std::cout << (int)(t->_process._priority) << std::endl;
			std::cout << t->_process._class << std::endl;
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
		qt = new QUEUE[sz];
	}
	
	void allocatePCB(){

	}

	void setUpPCB(std::string name, char priority, int CLASS){
	
	}

	int findPCB(){
	}

	void insertPCB(){
	}

	void removePCB(){
	}

};

PCB pcb;