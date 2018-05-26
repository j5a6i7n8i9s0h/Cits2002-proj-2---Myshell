#include "myshell.h"

#define COLON ':' // the separator

/*
    This function takes two arguments, a character pointer and a boolean variable
    In the case where iscd is false, it updates the path for a given Unix function to its full path.
    In the case where iscd is true, it finds the path for a given input
    If the given Unix function does not exist, it returns without changing the path, and prints an error message.
*/

bool SearchPath(char *t) 
{
  if(t[0]=='/') return true; // if there is a slash in the input, nothing is returned
  char* path= NULL; 
  path = strdup(DEFAULT_PATH); // sets the current path to the default path
  char *start=path;
  char *ret; // current path to check
  bool NoColons=false;
  bool added = false;
  while(!NoColons) // while there are colons in the expression
  {
      ret= strrchr(path,COLON); // change the current path to test
      if(ret!=NULL){ 
        ret++; // change what ret points to by one 
      }
      else
      {
        ret=start;
        NoColons=!NoColons;
      }
      strcat(ret,"/"); // add a slash to the path to test
      strcat(ret,t); // complete the potential command by adding the input value
      if(access(ret,F_OK|X_OK)!=-1) // test if the current command to be checked exists in Unix
      {
          strcpy(t,ret); // copy ret into t
          added = true;
      }
      else if(!NoColons) { // if there are colons, continue the process
          ret--;
          *ret='\0';
          ret=start;
      }
  }
    if(!added){ // if the command was not found
        fprintf(stderr,"Directory does not exist.\n"); // error message
        return false;
    }
    return true;
}

