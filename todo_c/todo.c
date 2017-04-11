#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "todo.h"

char *actions_str[] = {"list", "help", "status", "add", "rm"};

int array_length(void **array) {
  int length = 0;
  for (int i = 0; array[i]; i++){
    length++;
  }
  return length;
}

Task createTask(char *string){
  Task task = malloc(sizeof(Task) * 2);
  task->name = malloc(sizeof(char) * BUFFER_SIZE);
  char *buffer;
  char *separator = ";";

  string[strlen(string) - 1] = '\0';
  buffer = strtok(string, separator);
  while (buffer) {
    if (strlen(buffer) == 1){
      task->status = (buffer[0] == '1');
    }
    else {
      task->name = strcpy(task->name, buffer);
    }
    buffer = strtok(NULL, separator);
  }
  return task;
}

void print_tasks(TodoList *tl){
  for(int i = 0; i < tl->num_tasks; i++){
    printf("[%i] : %s\n", tl->tasks[i]->status, tl->tasks[i]->name);
  }
}

TodoList *parseFile(){
  char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
  char *todolist_file = strcat(getenv("HOME"), "/.todolist");
  TodoList *tl = malloc(sizeof(TodoList) * 2);
  tl->tasks = malloc(sizeof(Task) * BUFFER_SIZE);
  size_t len = 0;
  FILE *fp = fopen(todolist_file, "r");
  int i = 0;
  
  if (!fp){
    printf("%s : no such file\n", todolist_file);
    exit(-1);
  }
  while (getline(&buffer, &len, fp) != -1){
    tl->tasks[i++] = createTask(buffer);
  }
  tl->tasks[i] = NULL;
  tl->num_tasks = i;
  return tl;
}


Params handle_params(int argc, char **argv){
  Params tmp;
  int num_actions = array_length((void**)actions_str);
  tmp.action = LIST;
  if (argc >= 2) {
    for (int i = 0; i < num_actions; i++){
      if (strcmp(actions_str[i], argv[1]) == 0)
	tmp.action = i;
    }
  }
  return tmp;
}

void list(char **argv){
  TodoList *tl = parseFile();
  print_tasks(tl);
}

void help(char **argv){
  printf("usage : %s [list|help|add|rm|status] task\n", argv[0]);
}

void add(char **argv){
}

void rm(char **argv){
}

void status(char **argv){
}


Params params;
void (*actions[5])(char**) = {list, help, status, add, rm};

int main(int argc, char **argv){
  params = handle_params(argc, argv);
  (*actions[params.action])(argv);
}
