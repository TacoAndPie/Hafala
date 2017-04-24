#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <deque>
#include <list>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_ARG 20
#define MAX_HISTORY 50
typedef enum { FALSE , TRUE } bool;
typedef enum {FAILURE, SUCCESS} Result;
char* current_path;
char* previous_path = NULL;


int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(void* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

class cmdHistory {
public:
	cmdHistory();
};
#endif

