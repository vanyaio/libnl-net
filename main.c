#include "link.h"
#include "conf.h"
#include "netdevs.h"
#include "buffs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <sys/mount.h>

struct conf_file* config;
char* conf_path;
int node_num;
int set_root();
int mount_ns = 0;

int main_node(void* arg){
	printf("%s\n", "hello");
	struct node_entry* this_node_entry = &(config->entries[node_num]);
	sethostname(this_node_entry->hostname, strlen(this_node_entry->hostname));
	read_setdevs_pipe();
	printf("%s\n", "hello1");
	netdevs_set_node(this_node_entry, node_num);
	write_set_node_pipe();
	printf("%s\n", "hello2");
	read_exec_node_pipe();
	printf("%s\n", "hello3");
	int err = execvp(this_node_entry->task, conf_node_task_arg(this_node_entry, conf_path, node_num));
	printf("%s\n", "hello4");
	if (err)
		printf("error!!! %s\n", strerror(errno));
}

int main_userns(void* arg){
	read_euid_pipe();
	conf_set_etc_hosts(config);
	mount_var_run();

	//if (fork()==0)
		//system("/bin/bash");
	pid_t* node_pids = malloc(config->node_cnt * sizeof(pid_t));
	for (int i = 0; i < config->node_cnt; i++){
		node_num = i;
		node_pids[i] = clone(main_node, child_stack + STACK_SIZE,  CLONE_NEWNET | CLONE_NEWUTS | SIGCHLD, NULL);

		printf("pid: %d\n", node_pids[i]);
		if (mount_ns){
			char* ns_name = get_ns_name(node_num);
			char buff1[BUFF_SIZE];
			strcpy(buff1, "/var/run/netns/");
			strcat(buff1, ns_name);

			char pid_str[BUFF_SIZE];
			sprintf(pid_str, "%ld", node_pids[i]);
			char buff2[BUFF_SIZE];
			strcpy(buff2, "/proc/");
			strcat(buff2, pid_str);
			strcat(buff2, "/ns");
			strcat(buff2, "/net");
			//
			char buff4[BUFF_SIZE];
			strcpy(buff4, "ls -la ");
			strcat(buff4, buff2);
			system(buff4);
			//



			char buff3[BUFF_SIZE];
			strcpy(buff3, "touch ");
			strcat(buff3, buff1);
			system(buff3);
			//
			strcpy(buff4, "ls -la ");
			strcat(buff4, buff1);
			system(buff4);

			printf("%s\n", buff2);
			printf("%s\n", buff1);

			mount(buff2, buff1, NULL, MS_BIND, NULL);
			printf("code: %d\n\n\n\n\n", errno);

		}
	}
	printf("%s\n", "hello kek");
	netdevs_set_devs(config->node_cnt, node_pids);
	printf("%s\n", "hello keks1");
	write_setdevs_pipe(config->node_cnt);
	printf("%s\n", "hello kek1");
	read_set_node_pipe(config->node_cnt);
	write_exec_node_pipe(config->node_cnt);
	printf("%s\n", "hello kek2");
	int err = execvp(config->reaper, conf_reaper_arg(config, node_pids));
	printf("%s\n", "hello kek3");
	if (err == -1)
		printf("error! main_userns!! %s\n", strerror(errno));
}

int main(int c, char* argv[]){
	if (c > 3)
		mount_ns = 1;

	chdir(argv[2]);
	conf_path = argv[1];
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

int mount_var_run(){
	system("rm -rf tmp1");
  system("mkdir tmp1");
  system("cp -r /var/run/* tmp1");
  system("mount --bind tmp1 /var/run");
	system("rm -rf /var/run/netns");
	system("mkdir /var/run/netns");

  return 1;
}
int set_root(pid_t ns_pid){
	char pid_str[BUFF_SIZE];
	sprintf(pid_str, "%ld", ns_pid);
	char euid_str[BUFF_SIZE];
	sprintf(euid_str, "%d", geteuid());
	/*
	char* echo = concat(5, "echo \'0 ", euid_str, " 1\' > /proc/", pid_str, "/uid_map");
	*/
	char echo[BUFF_SIZE];
	strcpy(echo, "echo '0 ");
	strcat(echo, euid_str);
	strcat(echo, " 1' > /proc/");
	strcat(echo, pid_str);
	strcat(echo, "/uid_map");
	system(echo);
	//sfree(echo);
}
