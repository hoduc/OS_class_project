#pragma once
//cross-platform code
#if defined (__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#include "windows.h"
#define OS_WIN
#else
#include "sys/unistd.h"
#endif

enum RETURN_STATE{OK = 1, NOT_OK=-1};

typedef enum {APP, SYS} CLASS;

//state of a process
//also to figure out the queue in
typedef enum{
	READY, BLOCKED, SUSPENDED_READY, SUSPENDED_BLOCKED,
RUNNING, SUSPENDED
}STATE;
typedef int PRIORITY;
