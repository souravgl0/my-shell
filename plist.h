#ifndef plist_h
#define plist_h

typedef struct running_jobs
{
  char jobName[100];
  pid_t pid;
  struct running_jobs * next;
}running_jobs;

// running_jobs *rjobs;

int add_job(pid_t pid, char * name);
int delete_job(pid_t pid);

#endif
