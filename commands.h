#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_HISTORY_SIZE 50
typedef enum { FALSE, TRUE } bool;
typedef enum { FOREGROUND, BACKGROUND, INTERRUPTED } currState;
typedef enum { FAILURE, SUCCESS } Result;

using namespace std;

int ExeComp(char* lineSize, char* cmdString);
int BgCmd(char* lineSize, char* cmdString);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

//**************************************************************************************
// Class Name: cmdHistory
// cmdHistory object holds previous commands.
// Has the ability to: update and print its contents. 
//**************************************************************************************
class cmdHistory
{
public:
	cmdHistory();
	~cmdHistory();
	std::string get_pwd()
	{
		return PWD;
	}
	std::string get_prev_pwd()
	{
		return prevPWD;
	}
	void print();
	void updateHistory(std::string cmdString);
	void updateCurrAndPrev(std::string new_pwd);
	void swapCurrAndPrev();

private:
	std::string cmdHistoryArr[MAX_HISTORY_SIZE];
	std::string PWD;
	std::string prevPWD;
	const int historyCount;
};

//**************************************************************************************
// Class Name: Job
// According to the definition of the project Job objects will contain:
// PID,NAME,TIME SPENT IN JOB LIST, WHERE THE JOB IS CURRENTLY RUNNING (BG,FG,INTERRUPTED)
//**************************************************************************************
class Job
{
public:
	Job() : pid(-1), name(""), TimeInList((time_t)0), curr_state(background) {}; //default overwrite
	Job(int pid_, std::string name, time_t time, currState state) : pid(pid_), jobname(name), TimeInList(time), currState(state) {};
	virtual ~Job() {};

	int pid;
	std::string name;
	time_t TimeInList;
	currState curr_state
};
/* started by using struct but easier to use class.
typedef struct _Job {
	int pid;
	std::string name;
	time_t startingTime;
	currState curr_state;
};*/


//**************************************************************************************
// Name: cmdHistory
// Description: Constructor
// Parameters: none
// Returns: Initialized object
//**************************************************************************************
cmdHistory::cmdHistory() : historyCount(0), PWD(""), prevPWD("")
{
	char buffer[MAX_LINE_SIZE];//in case of failure, save in temp buffer
	if (getcwd(buffer, MAX_LINE_SIZE + 1) == NULL)
	{
		perror("smash error : > current working directory does not exist"); //send error message
	}
	else
	{
		PWD = (std::string)buffer;
		prevPWD = PWD;
	}
}


//**************************************************************************************
// Name: updateHistory
// Description: updates our history array by adding the current command and using FIFO
//				protocol if our array is full. 
// Parameters: string cmdString
// Returns: none
//**************************************************************************************
void cmdHistory::updateHistory(std::string cmdString)
{
	
	if (cmdString.empty())	return;
	if (historyCount == MAX_HISTORY_SIZE) // if history array is full we use FIFO
	{
		for (int i = 0; i<MAX_HISTORY_SIZE-1; i++)
		{
			cmdHistoryArr[i] = cmdHistoryArr[i + 1];
		}
		cmdHistoryArr[MAX_HISTORY_SIZE] = cmdString;
	}
	else
	{
		cmdHistoryArr[historyCount] = cmdString;
		historyCount++;
	}
}

//**************************************************************************************
// Name: printHistory
// Description: print all commands in our history array
// Parameters: none
// Returns: none
//**************************************************************************************
void cmdHistory::print()
{
	for (int i = 0; i < historyCount; i++) {
		printf("%c\n", cmdHistoryArr[i]);
	}
}

//**************************************************************************************
// Name: updateCurrAndPrev
// Description: update the PWD and the previous PWD
// Parameters: string new_pwd
// Returns: none
//**************************************************************************************
void cmdHistory::updateCurrAndPrev(std::string new_pwd)
{
	prevPWD = PWD;
	//PWD = newPWD;
	char buffer[MAX_LINE_SIZE];//in case of failure, save in temp buffer
	if (getcwd(buffer, MAX_LINE_SIZE + 1) == NULL)
	{
		perror("smash error : > current working directory does not exist"); //send error message
		PWD = "";
	}
	else
	{
		PWD = (std::string)buffer;
	}
}

//**************************************************************************************
// Name: swapCurrAndPrev
// Description: in case of "-" we simply swap the current and previous PWD
// Parameters: none
// Returns: none
//**************************************************************************************
void cmdHistory::swapCurrAndPrev()
{
	std::string temp = PWD;
	if (PWD == "" || prevPWD == "")
	{
		perror("smash error : > can't swap non-existant PWD's");
		return;
	}
	PWD = prevPWD;
	prevPWD = temp;
}

#endif