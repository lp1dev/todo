#include <unistd.h>
#include <string.h>
#include <stdio.h>

char *actions_str[] = {"help", "status", "add", "rm"};

enum{
  HELP,
  STATUS,
  ADD,
  RM
};

typedef struct Task{
  char *name;
  int status;
} Task;

typedef struct TodoList{
  char *filename;
  Task *tasks;
} TodoList;

typedef struct Params{
  int action;
  char *buffer;
} Params;

int array_length(void **array) {
  int length = 0;
  for (int i = 0; array[i]; i++){
    length++;
  }
  return length;
}

Params handle_params(int argc, char **argv){
  Params tmp;
  int num_actions = array_length((void**)actions_str);
  tmp.action = HELP;
  if (argc >= 2) {
    for (int i = 0; i < num_actions; i++){
      if (strcmp(actions_str[i], argv[1]) == 0)
	tmp.action = i;
    }
  }
  return tmp;
}

void help(char **argv){
  printf("usage : %s [help|add|rm|status] task\n", argv[0]);
}

void add(char **argv){
}

void rm(char **argv){
}

void status(char **argv){
}

TodoList parseFile(){
}

Params params;
void (*actions[4])(char**) = {help, status, add, rm};

int main(int argc, char **argv){
  params = handle_params(argc, argv);
  (*actions[params.action])(argv);
}
