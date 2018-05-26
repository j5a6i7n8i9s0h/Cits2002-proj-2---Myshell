#include "myshell.h"

int processes[BUFSIZ];
int childArray[BUFSIZ];

void addPID(int pid) {
    int i = 0;
    while(processes[i] != 0) {
        //printf("This slot has been used: %i\n", i);
       i++;
    }
    //printf("This slot is available: %i\n", i);
    processes[i] = pid;
    //printf("The %d'th entry of processes has been set to %d\n",i, processes[i]);
}

void printArray(int a[]) {
    int i = 0;
    while(i < 10) {
        printf(" The %d th value of the array is %d\n", i, a[i]);
        i++;
    }
}

bool isParent(int pid) {
    return(processes[0] == pid);
}


void copyArray(int a[], int b[]) {
    int i = 0;
    while(b[i] != 0) {
        a[i] = b[i];
        i++;
    }
}

void killAllProcesses(void) {
    int i = 1;
    while(childArray[i] != 0) {
        printf(" The %i th value of the array is %i\n", i, processes[i]);
        if( kill(processes[i], 0)!=-1){
        kill(processes[i], SIGTERM);
        }i++;
    }
}

int pipes() {
    int retrievePID[2];
    if(pipe(retrievePID) == -1) { // try to pipe. If pipe is unsuccesful, return.
        perror("pipe()");
        return 1;
    }
    pid_t pidval;
    close(retrievePID[0]);
    pidval= getpid();
    write(retrievePID[1],&pidval,sizeof(pidval));
    close(retrievePID[1]);
    close(retrievePID[1]);
    read(retrievePID[0],&pidval,sizeof(pidval));
    return 0;

}


