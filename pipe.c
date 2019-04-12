#include "pipe.h"


int init_pipes(){
  pipe(euid_pipe);
  pipe(setdevs_pipe);
  pipe(set_node_pipe);
  pipe(exec_node_pipe);
  return 1;
}

int write_euid_pipe(){
  write(euid_pipe[1], "1", 2);
  return 1;
}
int read_euid_pipe(){
  read(euid_pipe[0], pipe_buf, 2);
  return 1;
}

int write_setdevs_pipe(int nodes_cnt){
  for (int i = 0; i < nodes_cnt; i++)
    write(setdevs_pipe[1], "2", 2);
  return 1;
}
int read_setdevs_pipe(){
  read(setdevs_pipe[0], pipe_buf, 2);
  return 1;
}

int read_set_node_pipe(int nodes_cnt){
  for (int i = 0; i < nodes_cnt; i++)
    read(set_node_pipe[0], pipe_buf, 2);
  return 1;
}
int write_set_node_pipe(){
  write(set_node_pipe[1], "3", 2);
  return 1;
}

int read_exec_node_pipe(){
  read(exec_node_pipe[0], pipe_buf, 2);
  return 1;
}
int write_exec_node_pipe(int nodes_cnt){
  for (int i = 0; i < nodes_cnt; i++)
    write(exec_node_pipe[1], "4", 2);
  return 1;
}
