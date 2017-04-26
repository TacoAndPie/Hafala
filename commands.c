//		commands.c
//********************************************
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_HISTORY_SIZE 50
#define MAX_JOBS_NUM 100
typedef enum { FALSE, TRUE } bool;
typedef enum { FOREGROUND, BACKGROUND, INTERRUPTED } currState;
typedef enum { FAILURE, SUCCESS } Result;
#include "commands.h"

cmdHistory CommandHistory;
Job ForeGroundJob; //represents the only Job that will be in the foreground at a time.
Job* JobList;


//**************************************************************************************
// function name: RemoveKilledJob
// Description: Updates joblist after a job is killed
// Parameters: pointer to JobList (used STL list for convenience)
// Returns: none
//**************************************************************************************
void RemoveKilledJobs(std::list<Job>* JobList)
{
	std::list<Job>::iterator it = JobList->begin();
	while (it != JobList->end())
	{
		if (waitpid(it->pid, NULL, WNOHANG) != 0)
		{
			JobList->erase(it++);
		}
		else
			++it;
	}
}
}
//**************************************************************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(Job JobList, char* lineSize, char* cmdString) //void* jobs
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	
	
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) // change directory
	{
		if (num_arg != 1)	illegal_cmd = TRUE;
		else
		{
			if (!strcmp(args[1], "-"))
			{
				CommandHistory.swapCurrAndPrev();
			}
			else if (chdir(args[1]) == 0)// successfully changed directories
			{
				CommandHistory.updateHistory(args[1]);//or getcwd?
			}
			else
			{
				printf("smash error : > %s -path not found\n", args[1]);
				return 1;
			}
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if (num_arg != 0)	illegal_cmd = TRUE;
		else if (CommandHistory.get_pwd().empty())
		{
			perror("smash error : > pwd has not been set"); 
			return 1;
		}
		else
		{
			printf("%s\n", CommandHistory.get_pwd());
		}
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))// DO WE NEED?
	{
		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		if (num_arg != 0) 
		{
			illegal_cmd = TRUE;
		}
		else
		{
			Job* temp = JobList;
			time_t now;
			int i = 1;
			if (time(&now) == -1) 
			{
				perror("smash error: > time not found");
				return 0;
			}
			while (temp) 
			{ 
				int runTime = difftime(now, temp->startTime); 
				if (temp->curr_state == INTERRUPTED) {
					printf("[%d] %s : %d %d secs (Stopped)\n", i, temp->name, temp->pid, runTime);
				}
				else {
					printf("[%d] %s : %d %d secs\n", i,	temp->name, temp->pid, runTime);
				}
				temp = temp->next;
				i++;
			}
		}
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if (num_arg != 0)
		{
			illegal_cmd = TRUE;
		}
		else
		{
			printf("smash pid is %d\n", (int)getpid());
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if ((num_arg != 0 && num_arg != 1) || (num_arg == 1 && args[1] != "kill"))
		{
			illegal_cmd = TRUE;
		}
		else if (num_arg == 0)// exit smash
		{
			exit(0)
		}
		else // args[1] == "kill"
		{
			Job* temp = JobList;
			while(temp != NULL)
			{
				kill(temp.pid, SIGTERM);
				fprintf(stdout, "[%d] %s - Sending SIGTERM...");
				if (kill(temp.pid, 0) != -1) // haven't terminated process yet
				{
					sleep(5); // if still alive after 5 seconds, send SIGKILL
					if (waitpid(temp.pid, NULL, WNOHANG) != temp.pid)
					{
						fprintf(stdout, "(5 sec passed) Sending SIGKILL...");
						kill(temp.pid, SIGKILL);
						fprintf(stdout, "Done.");
					}
				}
				else	fprintf(stdout, "Done.");
				temp = temp.next;
			}
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))// use RemoveKilledJob at end
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		if (num_arg != 0)
		{
			illegal_cmd = TRUE;
		}
		else
		{
			CommandHistory.print();
		}
	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
				perror("smash error:> fork failed");
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

//**************************************************************************************
// function name: History
// Description: Constructor
// Parameters: none
// Returns: history object initialized to empty history, and PWD, and LastPWD are set
//**************************************************************************************
cmdHistory::cmdHistory() : historyCount(MAX_HISTORY_SIZE), PWD(""), prevPWD("")
{
	char pwd_buff[MAX_LINE_SIZE];//in case of failure, save in temp buffer
	if (getcwd(pwd_buff, MAX_LINE_SIZE + 1) == NULL)
	{
		perror("smash error : > Can't find current working directory"); //send error message
	}
	else
	{
		PWD = (std::string)pwd_buff;
		LastPWD = PWD;
	}
}


//**************************************************************************************
// function name: updateHistory
// Description: add the last cmd to the history, including illegal/failed ones
// Parameters: command string
// Returns: none
//**************************************************************************************
void cmdHistory::updateHistory(std::string cmdString)
{

	if (cmdString.empty())	return;

	cmdHistoryArr.push_back(cmdString);
	if ((int)HistoryList.size() > maxhistory) HistoryList.pop_front();
}

//**************************************************************************************
// function name: printHistory
// Description: print all commands in our history array
// Parameters: none
// Returns: none
//**************************************************************************************
void cmdHistory::print()
{
	for (int i = 0; i < this->historyCount; i++) {
		printf("%c\n", this->cmdHistoryArr[i]);
	}
}

//**************************************************************************************
// function name: setPWDandLastPWD
// Description: update the PWD and the LastPWD
// Parameters: newPWD
// Returns: none
//**************************************************************************************
void History::setPWDandLastPWD(std::string newPWD)
{
	LastPWD = PWD;
	//PWD = newPWD;
	char pwd_buff[MAX_LINE_SIZE];//in case of failure, save in temp buffer
	if (getcwd(pwd_buff, MAX_LINE_SIZE + 1) == NULL)
	{
		perror("smash error : > Can't find current working directory"); //send error message
		PWD = "";
	}
	else
	{
		PWD = (std::string)pwd_buff;
	}
}
