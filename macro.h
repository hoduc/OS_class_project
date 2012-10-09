#pragma once
enum RETURN_STATE{OK = 1, NOT_OK=-1};

typedef enum {APP, SYS} CLASS;

//state of a process
//also to figure out the queue in
typedef enum{
	READY, BLOCKED, SUSPENDED_READY, SUSPENDED_BLOCKED,
RUNNING, SUSPENDED
}STATE;
typedef int PRIORITY;
