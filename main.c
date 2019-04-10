#include "link.h"
#include "debug.h"
#include <errno.h>
#include "conf.h"
#include "netdevs.h"

conf_file* config;
int node_num;

int main_node(void* arg){
	struct node_entry* this_node_entry = &(config->entries[node_num]);
	read_setdevs_pipe()
	set_devices_node(this_node_entry)
	execvp(this_node_entry->task, conf_node_task_arg(node_entry));
}

int main_userns(void* arg){
	read_euid_pipe();

	pid_t* node_pids;
	for (int i = 0; i < conf_nodes_count; i++){
		node_num = i;
		node_pids[i] = clone(main_node, CLONE_NEWNET, null);
	}

	set_devices(conf_nodes_count);

	write_setdevs_pipe();

	execvp(config->reaper, conf_reaper_arg(config, node_pids));
}

int main(int c, char* argv[]){
	conf_alloc(&config, argv[1]);

	clone(main_userns, CLONE_NEWUSER | CLONE_NEWNET, argv)

	set_root();
	write_euid_pipe()

	//somehow get exit code of main_userns
	return 0;
}
