#ifndef struct_h
#define struct_h

typedef enum { false, true } bool;

typedef struct cmd
{
  char **args;
  bool bg;
  bool pipe;
}cmd;

#endif
