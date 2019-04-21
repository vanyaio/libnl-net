#ifndef NETDEVS_H
#define NETDEVS_H

#include "link.h"
#include "buffs.h"
#include "conf.h"
#define IN_NS 1

int netdevs_set_devs(int conf_nodes_count, pid_t* node_pids);
int netdevs_set_node(struct node_entry* entry, int num);
char* netdevs_set_bridge();
int netdevs_set_veth(int num, pid_t pid, char* master_bridge);

char* get_veth_name(int i, int in_ns);
char* get_veth_addr(int i);
char* get_ns_name(int num);

#endif
