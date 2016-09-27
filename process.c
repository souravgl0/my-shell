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
  int i,fd[2],orig_stdout,orig_stdin;
  bool out_redirect=false,in_redirect=false;
  for(i=0;i<cmdcount;i++)
  {
    if(!*(commands[i].args))continue;

    // cd need to be dealt in parent process
    if(strcmp(*commands[i].args,"cd")==0){run_cd(commands[i],Home_Path);continue;}

    if(out_redirect)
    {
      //restore stdout
      if(dup2(orig_stdout,STDOUT_FILENO) == -1){perror("Duplicating File Descriptor Failed");exit(1);}
      if(close(orig_stdout)==-1){perror("Closing File Descriptor Failed");exit(1);}
      out_redirect = false;
      //change stdin
      orig_stdin=dup(STDIN_FILENO);
      if(dup2(fd[0],STDIN_FILENO)==-1){perror("Duplicating File Descriptor Failed");exit(1);}
      if(close(fd[0])==-1){perror("Closing File Descriptor Failed");exit(1);}
      in_redirect = true;
    }
    if(commands[i].pipe)
    {
      //change stdout
      if(pipe(fd)==-1){perror("Pipe failed"); exit(1); }
      orig_stdout=dup(STDOUT_FILENO);
      if(dup2(fd[1],STDOUT_FILENO)==-1){perror("Duplicating File Descriptor Failed");exit(1);}
      if(close(fd[1])==-1){perror("Closing File Descriptor Failed");exit(1);}
      out_redirect=true;
    }

    pid_t pid=fork();
    if(pid<0){ perror("Could Not Fork "); exit(1); }
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

    if(in_redirect)
    {
      // restore stdin
      if(dup2(orig_stdin,STDIN_FILENO)==-1){perror("Duplicating File Descriptor Failed");exit(1);}
      if(close(orig_stdin)==-1){perror("Closing File Descriptor Failed");exit(1);}
      in_redirect = false;
    }

  }
}
