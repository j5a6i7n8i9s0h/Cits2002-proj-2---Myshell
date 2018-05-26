#include "myshell.h"
int lastexecstatus;
pid_t finished_cpid;
int retrieveArray[2];
int childArray[BUFSIZ];
bool isBackground;

/*
    This function executes a simple command that is input into the shell.
    It uses a combination of fork and exec, along with other functions created
    as part of this project.
*/
int simplecommand(SHELLCMD *t) {
    pid_t pid;
    int status;
    int retrievePID[2];
    if(pipe(retrievePID) == -1) { // try to pipe. If pipe is unsuccesful, return.
        perror("pipe()");
        return 1;
    }
    pid_t pidval;
    //printf("Process running the second fork: %d\n", getpid());
        if(!isInternal(t)) { // if this is an external Unix command
        switch(pid=fork()) // create new process
        {
            case -1:
                perror("fork()");
                exit(1);
            case 0:
                close(retrievePID[0]);
                pidval= getpid();
                write(retrievePID[1],&pidval,sizeof(pidval));
                close(retrievePID[1]);
                if(ioRedirect(t)!=0) { // if the output/input is redirected unsuccessfully
                    exit(1); // exit with failure
                }
                /*
                    The following line attempts to execute the input. If it does
                    not execute, it will assume that the input is a shellscript
                    and execute each line of the shellscript. See 8_shellscripts.c 
                    for more on this.
                */
                status = exec_shellscript(t);
                _exit(1); // exit the child process
                break;

            default:
                close(retrievePID[1]);
                read(retrievePID[0],&pidval,sizeof(pidval));
                //printf("Child process of second fork: %d\n", pidval);
                //printf("Currently we have pid: %d\n", getpid());
                if(!isParent(getpid())) {
                    addPID(getpid());
                    addPID(pidval);
                }
                if(isBackground){
                    close(retrieveArray[0]);
                    copyArray(childArray, processes);
                    write(retrieveArray[1],&childArray,sizeof(childArray));
                    close(retrieveArray[1]);
                }
                while(wait(&status)!=pid);// wait for the child process to finish executing
                break;
        }
        finished_cpid = pid;
    }
    else { // if this is an internal command from myshell
        lastexecstatus = execute_internal(t);
        if(WIFEXITED(lastexecstatus)) {
            return WEXITSTATUS(lastexecstatus);
        } // execute the command
    }
    return lastexecstatus=WEXITSTATUS(status); // returns the value corresponding to the success or failure of this function
}
