#define _POSIX_SOURCE
#include "myshell.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t finished_cpid;

/*
    This function handles the signal that is sent in the child process.
    It waits until all background processes are done and then jumps to the parent.
*/


void signal_handler(int sig) {
    bool statement = false;
    if(wait(NULL)==-1){
        fprintf(stdout,"%i process is finished\n",finished_cpid);
        statement = true;
    }
    while(wait(NULL)!=-1)
        ;
    if(!statement) fprintf(stdout,"%i process is finished\n",finished_cpid);
    //printf("This is the current child process ID: %d\n", getpid());
    //addPID(getpid());
    kill(getpid(),SIGTERM);
}

/*
    This function attempts to execute a background process.
    It registers a signal before forking, then places one process in the background
    before executing another.
    It returns the success value of the execution.
*/

int exec_background(SHELLCMD *node) {
    isBackground = true;
	int status = 0;
    pid_t pid;
    int retrievePID[2];
    if(pipe(retrievePID) == -1) { // try to pipe. If pipe is unsuccesful, return.
        perror("pipe()");
        return 1;
    }
    if(pipe(retrieveArray) == -1) {
        perror("pipe()");
        return 1;
    }
    pid_t pidval;
    //printf("Process ID before forking: %d\n", getpid());
    addPID(getpid());
    signal(SIGUSR1,signal_handler);
	 // signal is registered
	switch(pid = fork()) {
		case -1: // if fork is unsuccessful
			perror("fork()");
			exit(1);
		case 0: // child process
            close(retrievePID[0]);
            pidval= getpid();
            write(retrievePID[1],&pidval,sizeof(pidval));
            close(retrievePID[1]);
            if(execute_shellcmd(node->left)==1) status=1;
			kill(getpid(),SIGUSR1);
            _exit(1);
			break;

		default:
            close(retrievePID[1]);
            read(retrievePID[0],&pidval,sizeof(pidval));
            //printf("Child process ID before second fork:: %d\n",pidval);
            close(retrieveArray[1]);
            read(retrieveArray[0],&childArray,sizeof(childArray));
            status=execute_shellcmd(node->right);
			break;
    }
    printArray(childArray);
	return status;
} 

