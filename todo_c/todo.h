#ifndef _TODO_H_
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 2048
#define COLOR_GREEN "\033[1;32m"
#define COLOR_WHITE "\033[0;m"
#define COLOR_RED "\033[1;31m"
#define COLOR_NEUTRAL "\033[0m"
#endif

enum{
  LIST,HELP,STATUS,ADD,RM
};

typedef struct Params{
  int action;
  char *buffer;
} Params;

#endif

#ifndef _TASK_H_

void set_status(){
}

struct Task{
  char *name;
  int status;
  void (*set_status)();
} default_task = {"", 0, set_status};

typedef struct Task *Task;

#endif

#ifndef _TODOLIST_H_

struct TodoList{
  char *filename;
  Task *tasks;
  unsigned int num_tasks;
} default_todolist = {NULL, NULL};

typedef struct TodoList TodoList;
  
#endif
