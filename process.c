#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>

#include "parse.h"
#include "cd.h"
#include "struct.h"
#include "run.h"
#include "process.h"

void process(char Input[],int cmdcount,cmd commands[],char Home_Path[])
{
  int i,fd[2],orig_stdout,orig_stdin;
  bool out_redirect=false,in_redirect=false,pipe_out=false;
  for(i=0;i<cmdcount;i++)
  {
    if(!*(commands[i].args))continue;

    // cd need to be dealt in parent process
    if(strcmp(*commands[i].args,"cd")==0){run_cd(commands[i],Home_Path);continue;}

    //----------- Handling Piping-----------
    if(pipe_out)
    {
      // A pipe is in process - Changing Stdin to pipe output
      orig_stdin=dup(STDIN_FILENO);
      if(dup2(fd[0],STDIN_FILENO)==-1){perror("Duplicating File Descriptor Failed");return;}
      if(close(fd[0])==-1){perror("Closing File Descriptor Failed");return;}
      in_redirect = true;
      pipe_out = false;
    }
    if(commands[i].pipe)
    {
      //start pipe
      if(pipe(fd)==-1){perror("Pipe failed");  }
      //Change Stdout to pipe input
      orig_stdout=dup(STDOUT_FILENO);
      if(dup2(fd[1],STDOUT_FILENO)==-1){perror("Duplicating File Descriptor Failed");return;}
      if(close(fd[1])==-1){perror("Closing File Descriptor Failed");return;}
      out_redirect=true;
      pipe_out=true;
    }

    //----------Handling I/O Redirection------------------------
    if(commands[i].inredirect)
    {
      int fd_infile=open(commands[i].infile,O_RDONLY,0664);
      if(fd_infile==-1){perror("Error opening file");return;}
      orig_stdin=dup(STDIN_FILENO);
      if(dup2(fd_infile,STDIN_FILENO)==-1){perror("Duplicating File Descriptor Failed");return;}
      if(close(fd_infile)==-1){perror("Closing File Descriptor Failed");return;}
      in_redirect = true;
    }
    if(commands[i].outredirect)
    {
      int fd_outfile=creat(commands[i].outfile,0664);//@ ERROR handle
      orig_stdout=dup(STDOUT_FILENO);
      if(dup2(fd_outfile,STDOUT_FILENO)==-1){perror("Duplicating File Descriptor Failed");return;}
      if(close(fd_outfile)==-1){perror("Closing File Descriptor Failed");return;}
      out_redirect=true;
    }
    //---------- Fork and Run Child Process
    pid_t pid=fork();
    if(pid<0){perror("Could Not Fork ");return;}
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

    //----------- Restoring Stdout and Stdin after Piping
    if(in_redirect)
    {
      if(dup2(orig_stdin,STDIN_FILENO)==-1){perror("Duplicating File Descriptor Failed");return;}
      if(close(orig_stdin)==-1){perror("Closing File Descriptor Failed");return;}
      in_redirect = false;
    }
    if(out_redirect)
    {
      if(dup2(orig_stdout,STDOUT_FILENO) == -1){perror("Duplicating File Descriptor Failed");return;}
      if(close(orig_stdout)==-1){perror("Closing File Descriptor Failed");return;}
      out_redirect = false;
    }
  }
}
