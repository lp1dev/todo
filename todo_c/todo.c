#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "todo.h"

char *actions_str[] = {"list", "help", "status", "add", "rm"};
char *todolist_file;

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
    if (tl->tasks[i]->status)
      printf("[Task %i]: %s\n[Status]: %s%s%s\n\n", i, tl->tasks[i]->name, COLOR_GREEN, "Done", COLOR_NEUTRAL);
    else
      printf("[Task %i]: %s\n[Status]: %s%s%s\n\n", i, tl->tasks[i]->name, COLOR_RED, "Todo", COLOR_NEUTRAL);
  }
}

void dump_file(TodoList *tl){
  int fd = open(todolist_file, O_WRONLY);
  char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
  char *output = malloc(sizeof(char) * BUFFER_SIZE * tl->num_tasks);
  if (fd < 0){
    fd = open(todolist_file, O_APPEND);
    printf("[info] %s created\n", todolist_file);
  }
  for(int i = 0; i < tl->num_tasks; i++){
    if (strlen(tl->tasks[i]->name) > 0){
      memset(buffer, 0, strlen(buffer));
      sprintf(buffer, "%s;%i\n", tl->tasks[i]->name, tl->tasks[i]->status);
      strcat(output, buffer);
    }    
  }
  write(fd, output, strlen(output));
  close(fd);
}

TodoList *parseFile(){
  char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
  TodoList *tl = malloc(sizeof(TodoList) * 2);
  tl->tasks = malloc(sizeof(Task) * BUFFER_SIZE);
  size_t len = 0;
  FILE *fp = fopen(todolist_file, "r");
  int i = 0;
  
  if (!fp){
    fp = fopen(todolist_file, "a+");
    printf("[info] %s created\n", todolist_file);
  }
  while (getline(&buffer, &len, fp) != -1){
    tl->tasks[i++] = createTask(buffer);
  }
  tl->tasks[i] = NULL;
  tl->num_tasks = i;
  fclose(fp);
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

int list(int argc, char **argv, TodoList *tl){
  print_tasks(tl);
  return 0;
}

int help(int argc, char **argv, TodoList *tl){
  printf("usage : %s [list|help|add|rm|status] task\n", argv[0]);
  return 0;
}

int add(int argc, char **argv, TodoList *tl){
  if (argc != 3){
    help(argc, argv, tl);
    return -1;
  }
  if (strlen(argv[2]) > 1){
    Task task = malloc(sizeof(Task) * 2);
    task->name = argv[2];
    task->status = 0;
    tl->tasks[tl->num_tasks] = task;
    tl->num_tasks++;
    dump_file(tl);
  }
  print_tasks(tl);
  return 0;
}

int rm(int argc, char **argv, TodoList *tl){
  if (argc != 3){
    help(argc, argv, tl);
    return -1;
  }
  int num = atoi(argv[2]);
  if (num > -1 && num < tl->num_tasks){
    for (int i = num; i < tl->num_tasks; i++){
      tl->tasks[i] = tl->tasks[i+1];
    }
    tl->tasks[tl->num_tasks - 1] = NULL;
    tl->num_tasks--;
    dump_file(tl);
  }
  print_tasks(tl);
  return 0;
}

int status(int argc, char **argv, TodoList *tl){
    if (argc != 3){
      help(argc, argv, tl);
      return -1;
  }
  int num = atoi(argv[2]);
  if (num > -1 && num < tl->num_tasks){
    tl->tasks[num]->status = !(tl->tasks[num]->status);
    dump_file(tl);
  }
  print_tasks(tl);
  return 0;
}

int (*actions[5])(int, char**, TodoList *) = {list, help, status, add, rm};

int main(int argc, char **argv){
  todolist_file = getenv("HOME");
  strcat(todolist_file, "/.todolist");

  Params params;
  params = handle_params(argc, argv);
  return (*actions[params.action])(argc, argv, parseFile());
}
