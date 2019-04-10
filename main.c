#include "link.h"
#include "debug.h"
#include <errno.h>

conf_file* config;

int main_node(void* arg){
	//?get node_entry
	read_setdevs_pipe()
	set_devices_node(node_entry)
	execvp(conf_node_task(node_entry), conf_node_task_arg(node_entry));
}

int main_userns(void* arg){
	read_euid_pipe();

	pid_t* node_pids;
	for (int i = 0; i < conf_nodes_count; i++){
		node_pids[i] = clone(main_node, CLONE_NEWNET, conf_node_arg(i));

	set_devices(conf_nodes_count);

	write_setdevs_pipe();

	execvp(conf_reaper, conf_reaper_arg(node_pids));
}

int main(int c, char* argv[]){
	conf_alloc(config, argv[1]);
	
	clone(main_userns, CLONE_NEWUSER | CLONE_NEWNET, argv)

	set_root();
	write_euid_pipe()

	//somehow get exit code of main_userns
	return 0;
}
