#include "link.h"
#include "debug.h"

#define STACK_SIZE (1024 * 1024)
//static char child_stack[STACK_SIZE];
#define PRINT_BUFF_SIZE (1024 * 1024)
static char print_buff[PRINT_BUFF_SIZE];


int childFunc(void* arg){
	printf("ns-pid: %d\n", getpid());

  int i = *((int*)arg);
  char i_str[1000];
  sprintf(i_str, "%d", i+1);

  struct rtnl_link* veth = my_link_get_by_name(concat(2, "myveth_ns", i_str));
  if (veth == NULL)
    printf("null1\n");
  default_veth_up(veth);
  veth = my_link_get_by_name(concat(2, "myveth_ns", i_str));
  if (veth == NULL)
    printf("null2\n");
	//concat(4, "10.1.", i_str, ".2", "/24")
  default_addr_set2(veth, concat(3, "10.0.0.", i_str, "/16"));

  system("ip address show");
  while(1);
}

void script_inner(){
  printf("upper-pid: %d\n", getpid());

  int code = rtnl_link_bridge_add(get_route_socket(), "mybr");
	struct rtnl_link* br = my_link_get_by_name("mybr");
	default_bridge_up(br);
  br = my_link_get_by_name("mybr");
  //default_addr_set(br, "192.168.1.10/24");

  int ns_count = 2;
  pid_t* nss = malloc(ns_count * sizeof(pid_t));

  for (int i = 0; i < ns_count; i++){
    int* i_pass = malloc(sizeof(int));
    *i_pass = i;
    char* child_stack = malloc(STACK_SIZE);
    nss[i] = clone(childFunc,
             child_stack + STACK_SIZE,
             CLONE_NEWNET | SIGCHLD,
             i_pass);

    char i_str[1000];
    sprintf(i_str, "%d", i+1);
    rtnl_link_veth_add(get_route_socket(), concat(2, "myveth", i_str), concat(2, "myveth_ns", i_str), nss[i]);

    struct rtnl_link* veth = my_link_get_by_name(concat(2, "myveth", i_str));
  	if (veth == NULL)
  		printf("null1\n");
  	default_veth_up(veth);
    veth = my_link_get_by_name(concat(2, "myveth", i_str));
  	if (veth == NULL)
  		printf("null2\n");
    default_addr_set2(veth, concat(3, "10.1.0.", i_str, "/16"));

    struct nl_cache *cache;
  	struct nl_sock *sk1;
  	sk1 = nl_socket_alloc();
  	nl_connect(sk1, NETLINK_ROUTE);
  	if (rtnl_link_alloc_cache(sk1, AF_UNSPEC, &cache) < 0)
  		printf("error\n");

  	int master_index = rtnl_link_name2i(cache, "mybr");
  	if (!master_index)
  		printf("error master ind\n");
  	rtnl_link_set_master(veth, master_index);
  	rtnl_link_change(get_route_socket(), veth,
  									veth,
  									0);
  }


}
int main(int c, char* argv[]){
	script_inner();

	/*
    	pid_t child_pid = clone(childFunc,
                    child_stack + STACK_SIZE,
                    CLONE_NEWNET | SIGCHLD, NULL);
	printf("parent %d child from parent%d \n", getpid(), child_pid);
	*/


	/*
	struct nl_sock *sk;
	sk = nl_socket_alloc();
	nl_connect(sk, NETLINK_ROUTE);

	int code = rtnl_link_veth_add(sk, "ya1", "ya2", getpid());
	printf("adding code: %d\n", code);

	struct rtnl_link* link = my_link_get_by_name("ya1");
	if (link == NULL)
		printf("null\n");
	printf("here we are1\n");
	sleep(2);

	struct nl_addr** addr = malloc(sizeof(void*));
	//void* buf = malloc(6);
	//addr = nl_addr_build(AF_INET, buf, 6);
	code =  nl_addr_parse("10.1.1.1/24", AF_INET, addr);
	printf("parse code: %d\n", code);
	printf("here we are2\n");
	sleep(5);
	rtnl_link_set_addr(link, *addr);
	printf("here we are3\n");
	default_veth_up(link);

	print_flags(link);
	print_oper(link);
	*/


	/*
	int code = rtnl_link_veth_add(get_route_socket(), "ya1", "ya2", getpid());
	printf("adding code: %d\n", code);

	struct rtnl_link* link = my_link_get_by_name("ya1");
	if (link == NULL)
		printf("null\n");
	debug();
	default_veth_up(link);
	debug();
	*/



	/*
	debug();
	sleep(10);
	debug();
	*/




	/*
	struct nl_sock *sk1;
	sk1 = nl_socket_alloc();
	nl_connect(sk1, NETLINK_ROUTE);
	int code;
	code = rtnl_link_bridge_add(sk1, "mybrbrbr");
	printf("%d\n", code);
	*/



	/*
	int err;
	struct rtnl_link *link;

	if (!(link = rtnl_link_bridge_alloc()))
	   printf("here\n");

	//if(name)
	   //rtnl_link_set_name(link, name);

	err = my_rtnl_link_add(sk1, link, NLM_F_CREATE);
	rtnl_link_put(link);

	printf("%d\n", err);
	*/
	while(1);
	return 0;
}
