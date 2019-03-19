#include "link.h"
#include "debug.h"
#include <errno.h>

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];
#define PRINT_BUFF_SIZE (1024 * 1024)
static char print_buff[PRINT_BUFF_SIZE];
#define BUFF_SIZE 1024

#define IN_NS 1
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

char** _argv;
int _c;
int _i;
int childFunc(void* arg){
	sleep(2);
	char* name = get_veth_name(_i, IN_NS);
  struct rtnl_link* veth = my_link_get_by_name(name);
  if (veth == NULL)
    printf("null1\n");
  default_veth_up(veth);

  veth = my_link_get_by_name(name);
  if (veth == NULL)
    printf("null2\n");
  default_addr_set(veth, get_veth_addr(_i, IN_NS));


	if (fork() == 0)
		execvp(_argv[0], _argv);


  while(1);
}


int main(int c, char* argv[]){
	char br_name[] = "mybr";
	int code = rtnl_link_bridge_add(get_route_socket(), br_name);
	struct rtnl_link* br = my_link_get_by_name(br_name);

	//default_bridge_up(br);
	char* up_str = concat(3, "ip link set ", br_name, " up");
	system(up_str);
	free(up_str);

	br = my_link_get_by_name(br_name);
	default_addr_set(br, "10.0.0.1/16");


	int ns_count = 2;
	pid_t* nss = malloc(ns_count * sizeof(pid_t));

	_c = c-1;
	_argv = (argv + 1);
	for (int i = 0; i < ns_count; i++){
		_i = i;
		nss[i] = clone(childFunc,
		         child_stack + STACK_SIZE,
		         CLONE_NEWNET | SIGCHLD,
		         NULL);

		char* name = get_veth_name(i, 0);
		char* name_ns = get_veth_name(i, 1);
		rtnl_link_veth_add(get_route_socket(), name, name_ns, nss[i]);

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

	while(1);
	return 0;
}
