#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>

#include "parse.h"
#include "cd.h"
#include "struct.h"
#include "run.h"
#include "process.h"

void process(char Input[],int cmdcount,cmd commands[],char Home_Path[])
{
  int i;
  for(i=0;i<cmdcount;i++)
  {
    if(!*(commands[i].args))continue;
    if(strcmp(*commands[i].args,"cd")==0){run_cd(commands[i],Home_Path);continue;} // cd need to be dealt in parent process

    pid_t pid=fork();
    if(pid<0){ fprintf(stderr,"Error: Could Not Fork.\n"); exit(1); }
    if(pid==0)
    {
      if(commands[i].bg)  setpgid(0,0);
      runcommand(commands[i]);
      exit(0);
    }
    if(!commands[i].bg)
    {
      int status;
      waitpid(pid,&status,0);
    }
    else
    {
      printf("%s : Started with pid %d\n",*commands[i].args,pid);
    }
  }
}
