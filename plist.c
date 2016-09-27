#include<sys/types.h>
#include<stdlib.h>
#include<string.h>

typedef struct running_jobs
{
  char jobName[100];
  pid_t pid;
  struct running_jobs * next;
}running_jobs;

running_jobs *rjobs=NULL;

int add_job(pid_t pid, char * name)
{
	running_jobs *new=(running_jobs*)malloc(sizeof(running_jobs));
	new->pid=pid;
	strcpy(new->jobName,name);
	new->next=NULL;
	if (rjobs==NULL)
	{
		rjobs=new;
	}
	else
	{
		running_jobs *temp=rjobs;

		for (;temp->next!=NULL;temp=temp->next);
		temp->next=new;
	}
}

int delete_job(pid_t pid)
{
	if (rjobs==NULL) {
		return 1;
	}
	if (rjobs->pid==pid) {
		running_jobs *del=rjobs;
		rjobs=rjobs->next;
		free(del);
		return 0;
	}
	running_jobs *temp=rjobs,*prev;
	for (prev=rjobs;temp->pid!=pid || temp==NULL;temp=temp->next)
	{
		if (temp!=rjobs) {
		prev=prev->next;
	}
	}
	//running_jobs *del=temp->next;
	if(temp!=NULL)
	{
	prev->next=temp->next;
	free(temp);
  }
}
