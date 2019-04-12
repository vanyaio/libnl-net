#include "link.h"
#include "conf.h"
#include "netdevs.h"
#include "buffs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
//gcc -g -O0 link.c conf.c pipe.c netdevs.c main.c -o main $(pkg-config --cflags --libs libnl-3.0) -Wl,-rpath=/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -l:libnl-route-3.so.200
//gdb --args main apps/config
struct conf_file* config;
int node_num;
int set_root();

int main_node(void* arg){
	struct node_entry* this_node_entry = &(config->entries[node_num]);
	sethostname(this_node_entry->hostname, strlen(this_node_entry->hostname));
	read_setdevs_pipe();

	netdevs_set_node(this_node_entry, node_num);
	write_set_node_pipe();
	//
  //system("ip address show");
	//
	read_exec_node_pipe();
	int err = execvp(this_node_entry->task, conf_node_task_arg(this_node_entry));
	if (err)
		printf("error!!! %s\n", strerror(errno));
}

int main_userns(void* arg){
	read_euid_pipe();
	pid_t* node_pids = malloc(config->node_cnt * sizeof(pid_t));
	for (int i = 0; i < config->node_cnt; i++){
		node_num = i;
		node_pids[i] = clone(main_node, child_stack + STACK_SIZE,  CLONE_NEWNET | CLONE_NEWUTS | SIGCHLD, NULL);
	}

	netdevs_set_devs(config->node_cnt, node_pids);
	write_setdevs_pipe(config->node_cnt);

	read_set_node_pipe(config->node_cnt);
	write_exec_node_pipe(config->node_cnt);
	int err = execvp(config->reaper, conf_reaper_arg(config, node_pids));
	if (err == -1)
		printf("error! main_userns!! %s\n", strerror(errno));
}

int main(int c, char* argv[]){
	FILE *fp = fopen(argv[1], "r");
	init_pipes();
	conf_alloc(&config, fp);
	pid_t ns_pid = clone(main_userns,
											child_stack + STACK_SIZE,
											CLONE_NEWUSER | CLONE_NEWNET | CLONE_NEWNS | SIGCHLD,
											NULL);

	set_root(ns_pid);
	write_euid_pipe();

	int status;
	pid_t ret = waitpid(ns_pid, &status, 0);
	if (status != 0)
		exit(1);
	printf("main exits with 0\n");
	exit(0);
}




int set_root(pid_t ns_pid){
	char pid_str[BUFF_SIZE];
	sprintf(pid_str, "%d", ns_pid);
	char euid_str[BUFF_SIZE];
	sprintf(euid_str, "%d", geteuid());
	char* echo = concat(5, "echo \'0 ", euid_str, " 1\' > /proc/", pid_str, "/uid_map");
	system(echo);
	free(echo);
}
