#ifndef PIPE_H
#define PIPE_H

char pipe_buf[2];

int init_pipes();

int euid_pipe[2];
int write_euid_pipe();
int read_euid_pipe();

int setdevs_pipe[2];
int write_setdevs_pipe();
int read_setdevs_pipe();

int set_node_pipe[2];
int read_set_node_pipe();
int write_set_node_pipe();

int exec_node_pipe[2];
int read_exec_node_pipe();
int write_exec_node_pipe();

#endif
