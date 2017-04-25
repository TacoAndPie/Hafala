//		commands.c
//********************************************
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
#include "commands.h"

cmdHistory CommandHistory;

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
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
			else if (chdir(args[1]) == 0)//successfully changed directories
			{
				CommandHistory.updateHistory(args[1]);//or getcwd?
			}
			else
			{
				printf("smash error:> %s -path not found\n", args[1]);
				return 1;
			}
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		
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
