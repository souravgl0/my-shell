#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

#include "struct.h"

int run_cd(cmd command,char Home_Path[])
{
  char path[128];
  if(*(command.args+1)) strcpy(path,command.args[1]);
  else strcpy(path,"~");

  if(path[0]=='~')
  {
    strcpy(path,path+1);
    char temp[128];
    strcpy(temp,Home_Path);
    strcat(temp,path);
    strcpy(path,temp);
  }
  if(chdir(path)==-1)
  {
    if(errno==2)fprintf(stderr,"cd: %s : No such file or directory\n",path);
    else fprintf(stderr,"Error in changing directory: %d\n",errno);
    return -1;
  }
  return 1;
}
