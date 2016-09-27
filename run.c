#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>

#include "struct.h"
#include "cd.h"

int runcommand(cmd command)
{
  if(strcmp(*command.args,"echo")==0)
  {
    int i=1;
    while(*(command.args+i) !='\0'){printf("%s ",*(command.args+i));i++;}
    printf("\n");
  }
  else if(strcmp(*command.args,"pwd")==0)
  {
    char path[256];
    getcwd(path,256);
    printf("%s\n",path);
  }
  else if(execvp(*command.args,command.args)==-1)
  {
    if(errno==2)fprintf(stderr, "%s: Command not found...\n",*command.args);
    else {fprintf(stderr,"Error : %d\n",errno);}
  }
}
