#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "struct.h"

void initCommand(cmd * command)
{
  command->args=(char**)malloc(10*sizeof(char*));
  command->bg=false;
  command->pipe=false;
}
void parse(char * line,int *cmdcount,cmd commands[])
{
  initCommand(commands);
  int argind=0;
  bool next=true,end=false;
  *cmdcount=1;

  while(*line != '\0')
  {
    while(1)
    {
      // Handle Special Characters
      if( *line == ' ' || *line == '\t' || *line=='\n'){}
      else if( *line == '&'){commands->bg=true;}
      else if(*line=='"'){}
      else if( *line==';'){
        *(commands->args+argind)='\0';
        commands++;*cmdcount+=1;
        initCommand(commands);
        argind=0;
      }
      else if(*line=='|'){
        commands->pipe=true;
        *(commands->args+argind)='\0';
        commands++;*cmdcount+=1;
        initCommand(commands);
        argind=0;
      }
      else if(*line=='\0'){end=true;break;}
      else break;

      // Common to all special characters
      *line='\0';next=true;
      line++;
    }
    if(end)break;

    if(next)
    {
      next=false;
      *(commands->args + argind)=line;
      argind++;
    }
    line++;
  }
  *(commands->args+argind)='\0';
}
