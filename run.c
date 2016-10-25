#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<signal.h>

#include "struct.h"
#include "plist.h"

extern running_jobs *rjobs;

int toint(char * str)
{
  int len = strlen(str);
  int num=0,i;
  for(i=0; i<len; i++){
      num = num * 10 + ( str[i] - '0' );
  }
  return num;
}

void runcommand(cmd command)
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
  else if(strcmp(*command.args,"listjobs")==0)
  {
    int ind=1;
    running_jobs * head=rjobs;
    while(head)
    {
      printf("[%d] %s [%d]\n",ind,head->jobName,head->pid);
      head=head->next;ind++;
    }
  }
  else if(strcmp(*command.args,"sendsig")==0)
  {
    int i;
    char num[50];
    int pind = toint(*(command.args+1));
    int sigind = toint(*(command.args+2));

    running_jobs * head=rjobs;

    for(i=1;i<pind;i++)
    {
      if(head && head->next)head=head->next;
      else {fprintf(stderr, "Given pindex doesn't exist\n");return;}
    }
    kill(head->pid,sigind);
  }
  else if(strcmp(*command.args,"killallbg")==0)
  {
    running_jobs * head=rjobs;
    while(head)
    {
      kill(head->pid,9);
      head=head->next;
    }
  }
  else if(execvp(*command.args,command.args)==-1)
  {
    if(errno==2)fprintf(stderr, "%s: Command not found...\n",*command.args);
    else {perror("Error");}
  }
}
