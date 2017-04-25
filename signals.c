// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern int kill_cmd;
extern int GPID;
extern char* L_foreGround_Cmd;
extern LIST_ELEMENT* Jobs;

//**************************************************************************************	 	 		 	 
// function name: Signal_Printer	 	 	 
// Description: prints after every signal kill "signal <signum> was sent to pid <pid>"	 		 	
// Parameters: sig_num: Signal number, pid: PID number	 	 	
//**************************************************************************************	 	 	 
void Signal_Printer(int sig_num,int pid){
	std::string sigName;
	switch(sig_num){
		case 1 :
			sigName= "SIGHUP"; 
			break;
		case 2 :
			sigName= "SIGINT"; 
			break;
		case 3 : 
			sigName= "SIGQUIT"; 
			break;
		case 4 : 
			sigName= "SIGILL"; 
			break;
		case 5 :
			sigName= "SIGTRAP"; 
			break;
		case 6 : 
			sigName= "SIGABRT"; 
			break;
		case 7 :
			sigName= "SIGBUS"; 
			break;
		case 8 :
			sigName= "SIGFPE"; 
			break;
		case 9 :
			sigName= "SIGKILL"; 
			break;
		case 10 : 
			sigName= "SIGUSR1"; 
			break;
		case 11 : 
			sigName= "SIGSEGV"; 
			break;
		case 12 : 
			sigName= "SIGUSR2"; 
			break;
		case 13 :
			sigName= "SIGPIPE"; 
			break;
		case 14 :
			sigName= "SIGALRM"; 
			break;
		case 15 :
			sigName= "SIGTERM"; 
			break;
		case 17 : 
			sigName= "SIGCHLD"; 
			break;
		case 18 : 
			sigName= "SIGCONT"; 
			break; 
		case 19 :
			sigName= "SIGSTOP"; 
			break;
		case 20 : 
			sigName= "SIGTSTP";
			break;
		case 21 :
			sigName= "SIGTTIN"; 
			break;
		case 22 : 
			sigName= "SIGTTOU";
			break;
		case 23 : 
			sigName= "SIGURG"; 
			break;
		case 24 : 
			sigName= "SIGXCPU";
			break;
		case 25 : 
			sigName= "SIGXFSZ"; 
			break;
		case 26 : 
			sigName= "SIGVTALRM";
			break;
		case 27 :
			sigName= "SIGPROF"; 
			break;
		case 28 :
			sigName= "SIGWINCH";
			break;
		case 29 : 
			sigName= "SIGIO"; 
			break;
		case 30 : 
			sigName= "SIGPWR";
			break;
		case 31 : 
			sigName= "SIGSYS"; 
			break;
		default : 
			sigName= "UNKNOWN"; 	 	 	
	}
	cout<<"smash > signal "<<sigName<<" was sent to pid "<<pid<<endl;                  	 		 	 	 	 
	if (kill(pid, sig_num)!=0) { 	 	 		 	 		 	
		perror("smash error: > kill failed");                  	 			 	 	
	}
}






//**************************************************************************************	 	 	
// function name:SIGTSTP_Handler	 		 	 	
// Description:Handler for the signal for Ctrl + Z	(SIGTSTP)
// Parameters:sig_num: Signal number	 	 	
//**************************************************************************************	 	 	 


void SIGTSTP_Handler(){ 		 	 		 
	if (GPID<=0) 	 	 	 
	{ 		 	 
		if (kill_cmd!=-1) 	 	 	
		{	 	 	 
			Signal_Printer(SIGTSTP,kill_cmd); 	 	 
			LIST_ELEMENT* JobIt= Jobs;	 	 	 
			while (JobIt){	 	 	 
				if (JobIt->pID==kill_cmd) 	 	 	 
				{	 	 
					JobIt->suspended= 1;	 	 
				}	  	
				JobIt= JobIt->pNext;	 	 
			}	 	 
		} 	 	 	 
		
	} 	 	 
	else 	 	 
	{	 
		Signal_Printer(SIGTSTP, GPID);	 	 	
		if (GetId(&Jobs, GPid)==-1)  	 	 	 
		{ 	 	 		 	 
			if (InsertAtEnd(&Jobs,L_foreGround_Cmd,0,GPID,1)==1) 	 	 
			{	 		 	
				printf("smash error :> adding a Suspended job failed\n");	 	 	 
				exit(1);	 	 	 	
			} 	
		}	 	 
		else 	 	 		 
		{ 	 	 	 		  
			LIST_ELEMENT* JobIt= Jobs;	 		 	 	
			while (JobIt) { 	 	 	 	 
				if (JobIt->pID==GPID)  	 	 	
				{	 	 	 	
					JobIt->suspended= 1;	 	 	 	
				}	 	 	 	 
				JobIt= JobIt->pNext;	 	 	 
			}	 	 		 	 		 
		}	 	 	
		GPID= -1;	 	 	 
		L_foreGround_Cmd[0]= '\0';	 
	}
	return;
}



//**************************************************************************************	 	 		
// function name:SIGINT_Handler	 	 	
// Description:Handler for the signal for Ctrl + C (SIGINT) 
// Parameters:sig_num: -	 	 	 
//**************************************************************************************	 	 	 
void SIGINT_Handler(){
	
	I
	if (GPID <= 0) { 
		return;
	}
	Signal_Printer(SIGINT, GPID);	
	if(DelPID(&Jobs, GPID)==1)
	{
		printf("smash error :> deleting a job failed\n");
		exit(1);
	}
		
	GPID = -1;
	L_foreGround_Cmd[0] = '\0';
	return;
} 	 
