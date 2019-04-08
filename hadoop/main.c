#include "../link.h"
#include "../debug.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];
#define PRINT_BUFF_SIZE (1024 * 1024)
static char print_buff[PRINT_BUFF_SIZE];
#define BUFF_SIZE 1024

#define IN_NS 1
char* get_veth_name(int i, int in_ns);
char* get_veth_addr(int i, int in_ns);

pid_t ns_pid1;
pid_t ns_pid2;
int _i;

int setdevs_pipe[2];
int master_pipe[2];
int slave_pipe[2];
char pipe_buf[2];

extern int redir_io(const char* path, int is_out);
int set_devices_ns();
int set_hadoop_vars();
int set_slave_daemons();
void print_envs();
int slave(void* arg){
	read(master_pipe[0], pipe_buf, 2);

	char hname[] = "hadoop-dns-ns";
	sethostname(hname, strlen(hname));
	set_devices_ns();
	set_hadoop_vars();
	set_slave_daemons();

	printf("slave is set \n");
	sleep(5);
	//
	print_envs();
	//
	write(slave_pipe[1], "1", 2);

	sleep(1000000);
}


int set_slave_home();
int set_master_daemons();
int run_task();
int master(void* arg){
	read(setdevs_pipe[0], pipe_buf, 2);

	char hname[] = "hadoop-dns";
	sethostname(hname, strlen(hname));
	set_devices_ns();
	set_slave_home();
	set_master_daemons();

	printf("master is set \n");
	sleep(5);
	write(master_pipe[1], "1", 2);

	//printf("master's name\n");
	read(slave_pipe[0], pipe_buf, 2);
	//
	print_envs();
	//
	run_task();
	sleep(100000000);
}


int set_devices();
extern char* concat(int count, ...);
int euid_pipe[2];
int user_ns_func(void* arg){
	read(euid_pipe[0], pipe_buf, 2);
	printf("euid %ld\n", getuid());//echo '0 1000 1' > /proc/4713/uid_map

	pipe(setdevs_pipe);
	pipe(master_pipe);
	pipe(slave_pipe);

	_i = 0;
	ns_pid1 = clone(slave,
			child_stack + STACK_SIZE,
			CLONE_NEWNET | CLONE_NEWUTS | SIGCHLD,
			NULL);
	_i = 1;
	ns_pid2 = clone(master,
			child_stack + STACK_SIZE,
			CLONE_NEWNET | CLONE_NEWUTS | SIGCHLD,
			NULL);
	set_devices();

	write(setdevs_pipe[1], "1", 2);

	sleep(100000);
}

int main(int argc, char* argv[]){
	system("jps | python3 ../util/clean_hadoop_daemons.py");

	pipe(euid_pipe);

	pid_t ns_pid = clone(user_ns_func,
			child_stack + STACK_SIZE,
			CLONE_NEWUSER | CLONE_NEWNET | SIGCHLD,
			NULL);

	char pid_str[100];
	sprintf(pid_str, "%d", ns_pid);
	char* echo = concat(3, "echo \'0 1000 1\' > /proc/", pid_str, "/uid_map");
	printf("%s\n", echo);
	system(echo);
	write(euid_pipe[1], "1", 2);

	sleep(1000000);
	/*
	int euid = geteuid();
	system("echo ");
	*/
	return 0;
}


/*

utils

*/
int set_devices(){
	//bridge:

	char br_name[] = "mybr";
	/*
	int code = rtnl_link_bridge_add(get_route_socket(), br_name);
	if (code != 0)
		printf("EROR");
	struct rtnl_link* br = my_link_get_by_name(br_name);

	//default_bridge_up(br);
	char* up_str = concat(3, "ip link set ", br_name, " up");
	system(up_str);
	free(up_str);

	br = my_link_get_by_name(br_name);
	default_addr_set(br, "10.0.0.1/16");
	*/

	printf("euid %ld \n", (long) geteuid());
	system("ip link add name mybr type bridge");
	system("ip addr add 10.0.0.1/16 dev mybr");
	system("ip link set mybr up");

	//veth_pairs:
	{
		int i = 0;
		char* name = get_veth_name(i, 0);
		char* name_ns = get_veth_name(i, 1);
		rtnl_link_veth_add(get_route_socket(), name, name_ns, ns_pid1);

		struct rtnl_link* veth = my_link_get_by_name(name);
		if (veth == NULL)
			printf("null1\n");
		default_veth_up(veth);
		veth = my_link_get_by_name(name);
		if (veth == NULL)
			printf("null2\n");
		default_addr_set(veth, get_veth_addr(i, !IN_NS));

		struct nl_cache *cache;
		struct nl_sock *sk1;
		sk1 = nl_socket_alloc();
		nl_connect(sk1, NETLINK_ROUTE);
		if (rtnl_link_alloc_cache(sk1, AF_UNSPEC, &cache) < 0)
			printf("error\n");

		int master_index = rtnl_link_name2i(cache, br_name);
		if (!master_index)
			printf("error master ind\n");
		rtnl_link_set_master(veth, master_index);
		rtnl_link_change(get_route_socket(), veth,
										veth,
										0);
	}
	{
		int i = 1;
		char* name = get_veth_name(i, 0);
		char* name_ns = get_veth_name(i, 1);
		rtnl_link_veth_add(get_route_socket(), name, name_ns, ns_pid2);

		struct rtnl_link* veth = my_link_get_by_name(name);
		if (veth == NULL)
			printf("null1\n");
		default_veth_up(veth);
		veth = my_link_get_by_name(name);
		if (veth == NULL)
			printf("null2\n");
		default_addr_set(veth, get_veth_addr(i, !IN_NS));

		struct nl_cache *cache;
		struct nl_sock *sk1;
		sk1 = nl_socket_alloc();
		nl_connect(sk1, NETLINK_ROUTE);
		if (rtnl_link_alloc_cache(sk1, AF_UNSPEC, &cache) < 0)
			printf("error\n");

		int master_index = rtnl_link_name2i(cache, br_name);
		if (!master_index)
			printf("error master ind\n");
		rtnl_link_set_master(veth, master_index);
		rtnl_link_change(get_route_socket(), veth,
										veth,
										0);
	}
	printf("devs are set \n");
}

int set_devices_ns(){
	char* name = get_veth_name(_i, IN_NS);
	struct rtnl_link* veth = my_link_get_by_name(name);
	if (veth == NULL)
		printf("null1\n");
	default_veth_up(veth);

	veth = my_link_get_by_name(name);
	if (veth == NULL)
		printf("null2\n");
	default_addr_set(veth, get_veth_addr(_i, IN_NS));

	system("ip link set lo up");
	printf("devs are set in ns \n");
}

#define slave_home "/home/ivan/slave-home";
int set_hadoop_vars(){
	setenv("JAVA_HOME", "/usr/lib/jvm/java-11-oracle", 1);

	setenv("HADOOP_INSTALL", "/home/ivan/slave-home", 1);
	setenv("HADOOP_HOME", "/home/ivan/slave-home", 1);
	setenv("HADOOP_PREFIX", "/home/ivan/slave-home", 1);

	setenv("PATH", "/home/ivan/bin:/home/ivan/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/lib/jvm/java-11-oracle/bin:/usr/lib/jvm/java-11-oracle/db/bin:/usr/lib/jvm/java-11-oracle/bin:/home/ivan/slave-home/bin:/home/ivan/slave-home/sbin", 1);
	setenv("HADOOP_MAPRED_HOME", "/home/ivan/slave-home", 1);
	setenv("HADOOP_COMMON_HOME", "/home/ivan/slave-home", 1);
	setenv("HADOOP_HDFS_HOME", "/home/ivan/slave-home", 1);
	setenv("YARN_HOME", "/home/ivan/slave-home", 1);
	setenv("HADOOP_COMMON_LIB_NATIVE_DIR", "/home/ivan/slave-home/lib/native", 1);
	//setenv("HADOOP_OPTS", "\"-Djava.library.path=/home/ivan/slave-home/lib\"", 1);
	//setenv("HADOOP_OPTS","-Djava.library.path=/home/ivan/hadoop/lib");
	printf("vars are set\n");
	return 1;
}
void print_envs(){
	system("echo $JAVA_HOME");
	system("echo $HADOOP_INSTALL");
	system("echo $HADOOP_CONF_DIR");
	system("echo $PATH");
}
int set_slave_home(){
	system("rm -rf /home/ivan/slave-home");
	system("mkdir /home/ivan/slave-home");
	system("cp -r /home/ivan/hadoop/* /home/ivan/slave-home");
	system("rm -rf /home/ivan/slave-home/data/datanode/*");
	system("rm -rf /home/ivan/slave-home/data/namenode/*");
	system("rm -rf /home/ivan/slave-home/etc/hadoop/hdfs-site.xml");
	system ("cp hdfs-site.xml /home/ivan/slave-home/etc/hadoop");
	printf("slave home is set\n");
	return 1;
}

int set_slave_daemons(){
	int fd = dup(2);
	int mode = 2;

	//redir_io("logs/slave-datanode", mode);
	system("/home/ivan/slave-home/bin/hadoop datanode &");
	redir_io("logs/slave-nodemanager", mode);
	system("/home/ivan/slave-home/bin/yarn nodemanager &");

	close(2);
	dup2(fd, 2);
	printf("slave daemons set\n");
}

int set_master_daemons(){
	int mode = 2;
	int fd = dup(2);

	if (fork() == 0){
		redir_io("logs/master-namenode", mode);
		system("/home/ivan/hadoop/bin/hadoop namenode &");
		exit(0);
	}

	if (fork() == 0){
		redir_io("logs/master-datanode", mode);
		system("/home/ivan/hadoop/bin/hadoop datanode &");
		exit(0);
	}

	if (fork() == 0){
		redir_io("logs/master-resourcemanager", mode);
		system("/home/ivan/hadoop/bin/yarn resourcemanager &");
		exit(0);
	}
	if (fork() == 0){
		redir_io("logs/master-nodemanager", mode);
		system("/home/ivan/hadoop/bin/yarn nodemanager &");
		exit(0);
	}

	close(2);
	dup2(fd, 2);
	printf("master daemons set\n");
}

int run_task(){
	printf("SAFEMODE GETTING OFF\n\n\n\n\n");
	system("hdfs dfsadmin -safemode leave");
	if (fork() == 0){
		//system("chromium-browser --no-sandbox");
		sleep(100000000);
		exit(0);
	}
	sleep(5);
	printf("starting task! \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//system("yarn jar /home/ivan/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.1.jar pi 2 5");
	//system("hdfs dfs -mkdir out");

	//system("yarn jar /home/ivan/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.1.jar wordcount books out4");
	//system("hdfs dfs -ls /user/root/out3");
	//system("hdfs dfs -cat /user/root/out5/part-r-00000");

	//system("hdfs dfsadmin -report");
	system("/bin/sh");
}




//libnl part

char* get_veth_name(int i, int in_ns){
	char i_str[BUFF_SIZE];
	sprintf(i_str, "%d", i + 1);

	if (!in_ns)
		return concat(2, "myveth", i_str);

	return concat(2, "myveth_ns", i_str);
}
char* get_veth_addr(int i, int in_ns){
	char i_str[BUFF_SIZE];
	sprintf(i_str, "%d", i + 2);

	if (!in_ns)
		return concat(3, "10.1.0.", i_str, "/16");

	return concat(3, "10.0.0.", i_str, "/16");
}


























//
