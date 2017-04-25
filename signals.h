#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <vector>
#include <commands.h>
typedef enum { FALSE, TRUE } bool;
typedef enum { FOREGROUND, BACKGROUND, INTERRUPTED } currState;
typedef enum { FAILURE, SUCCESS } Result;

//**************************************************************************************	 	 		 	 
// function name:Signal_Printer	 	 	 
// Description:Prints signal kill "signal <signum> was sent to pid <pid>"	 		 	
// Parameters:sig_num: Signal number, pid: PID	 	 	
//**************************************************************************************	 	 	 
void Signal_Printer(int sig_num,int pid);				 		 	 



//**************************************************************************************	 	 	
// function name:SIGTSTP_Handler	 		 	 	
// Description:Handler for the signal for Ctrl + Z	(SIGTSTP)
// Parameters:-	 	 	
//**************************************************************************************	 	 	 
void SIGTSTP_Handler();	 	 	 		 



//**************************************************************************************	 	 		
// function name:SIGINT_Handler	 	 	
// Description:Handler for the signal for Ctrl + C (SIGINT) 
// Parameters:-	 	 	 
//**************************************************************************************	 	 	 
void SIGINT_Handler();	 	 	

#endif

