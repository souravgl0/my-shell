#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include "parse.h"
#include "cd.h"
#include "struct.h"
#include "run.h"
#include "process.h"

int calcpath(char Cur_Path[],char Home_Path[])
{
  size_t buf_size=64;
  while(getcwd(Cur_Path,buf_size)==NULL){ if(errno==ERANGE) buf_size*=2; else return -1;}
  if(strstr(Cur_Path,Home_Path)!=NULL)
  {
    memmove(Cur_Path,Cur_Path+strlen(Home_Path),strlen(Home_Path)+1);
    char temp[128];
    strcpy(temp,"~");
    strcat(temp,Cur_Path);
    strcpy(Cur_Path,temp);
  }
}

void ignore(){}
int main()
{
  char Login_Name[128],Host_Name[128],Cur_Path[128];
  char *Input=NULL;
  size_t inplen=0,buf_size=128;
  int i;

  signal(SIGINT,ignore); // Ignore Ctrl-C normally;

  while(getcwd(Cur_Path,buf_size)==NULL)
  {
    if(errno==ERANGE)buf_size*=2;
    else  {fprintf(stderr, "Error getting current path : %d\n",errno);  break; }
  }
  char *Home_Path=(char*)malloc(strlen(Cur_Path)*sizeof(char));
  strcpy(Home_Path,Cur_Path);

  do
  {
    if(gethostname(Host_Name,sizeof(Host_Name))==-1){fprintf(stderr,"Error getting System Name.\n");}
    if(getlogin_r(Login_Name,sizeof(Login_Name))==-1){fprintf(stderr,"Error getting User Name.\n");}
    if(calcpath(Cur_Path,Home_Path)==-1)fprintf(stderr,"Error getting current path : %d",errno);

    printf("[%s@%s %s] $ ",Login_Name,Host_Name,Cur_Path);
    if(getline(&Input,&inplen,stdin)==-1){ // Ctrl-D (EOF) --> Exit Shell
      printf("\n");
      exit(0);
    }

    cmd commands[10];
    int cmdcount;
    parse(Input,&cmdcount,commands);
    process(Input,cmdcount,commands,Home_Path);
    free(Input);
    for(i=0;i<cmdcount;i++)free(commands[i].args);
    Input=NULL;
  } while(1);
  return 0;
}
