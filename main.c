#include "conf.h"
#include "netdevs.h"
#include "buffs.h"

conf_file* config;
int node_num;
int set_root();

int main_node(void* arg){
	struct node_entry* this_node_entry = &(config->entries[node_num]);
	read_setdevs_pipe()

	netdevs_set_node(this_node_entry, node_num)
	write_set_node_pipe();

	read_exec_node_pipe()
	execvp(this_node_entry->task, conf_node_task_arg(node_entry));
}

int main_userns(void* arg){
	read_euid_pipe();

	pid_t* node_pids;
	for (int i = 0; i < conf_nodes_count; i++){
		node_num = i;
		node_pids[i] = clone(main_node, CLONE_NEWNET, null);
	}

	netdevs_set_devs(conf_nodes_count, pid_t* node_pids);
	write_setdevs_pipe();

	read_set_node_pipe()
	write_exec_node_pipe();
	execvp(config->reaper, conf_reaper_arg(config, node_pids));
}

int main(int c, char* argv[]){
	conf_alloc(&config, argv[1]);

	ns_pid = clone(main_userns, CLONE_NEWUSER | CLONE_NEWNET, argv)

	set_root(ns_pid);
	write_euid_pipe()

	int status;
	waitpid(ns_pid, &status, 0);
	if (status != 0 )
		exit(1);
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
