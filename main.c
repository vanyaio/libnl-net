#include "link.h"
#include "debug.h"
#include <errno.h>


int main_node(void* arg){
	read_setdevs_pipe()
	set_devices_node(arg)
	execvp(conf_node_task(arg), conf_node_task_arg(arg));
}

int main_userns(void* arg){
	read_euid_pipe();

	pid_t* node_pids;
	for (int i = 0; i < conf_nodes_count; i++){
		node_pids[i] = clone(main_node, CLONE_NEWNET, conf_node_arg(i));

	set_devices();

	write_setdevs_pipe();

	execvp(conf_task, conf_task_arg(arg, node_pids));
}

int main(int c, char* argv[]){
	clean_cluster?

	clone(main_userns, CLONE_NEWUSER | CLONE_NEWNET, argv)

	set_root();
	write_euid_pipe()

	//somehow get exit code of main_userns
	return 0;
}
