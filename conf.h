#ifndef CONF_H
#define CONF_H

#include "buffs.h"

#define NODE_ENTRY_FIELDS 3
struct node_entry{
  char* ip_addr;
  char* hostname;
  char* task;
};
struct conf_file{
  char* reaper;
  int node_cnt;
  struct node_entry* entries;
};

int conf_alloc(struct conf_file** conf, FILE* fp);
char* conf_reaper_arg(struct conf_file* conf, pid_t* node_pids);
char* conf_node_task_arg(struct node_entry* entry);

#endif
