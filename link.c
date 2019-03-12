#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>

#include <netlink/netlink.h>
#include <netlink/cache.h>
#include <netlink/route/link.h>
#include <netlink/socket.h>

#include <netlink/route/link/veth.h>

#include <netlink/types.h>

#include <sys/ioctl.h>
#include <net/if.h>
enum {
	IF_OPER_UNKNOWN,
	IF_OPER_NOTPRESENT,
	IF_OPER_DOWN,
	IF_OPER_LOWERLAYERDOWN,
	IF_OPER_TESTING,
	IF_OPER_DORMANT,
	IF_OPER_UP,
};

/*start*/

#define STACK_SIZE (1024 * 1024)
char child_stack[STACK_SIZE];
#define PRINT_BUFF_SIZE (1024 * 1024)
char print_buff[PRINT_BUFF_SIZE];

struct rtnl_link* my_link_get_by_name(const char* name){
	struct rtnl_link *link;
	struct nl_cache *cache;
	struct nl_sock *sk1;
	sk1 = nl_socket_alloc();
	nl_connect(sk1, NETLINK_ROUTE);
	if (rtnl_link_alloc_cache(sk1, AF_UNSPEC, &cache) < 0)
		printf("error\n");
	if (!(link = rtnl_link_get_by_name(cache, name)))
		printf("link does not exist\n");
	return link;
}

void print_flags(struct rtnl_link* link){
	unsigned int link_flags = rtnl_link_get_flags(link);
	char* flags_str = rtnl_link_flags2str(link_flags, print_buff, PRINT_BUFF_SIZE);
	printf("flags: %s\n", flags_str);
}

void print_oper(struct rtnl_link* link){
	uint8_t oper = rtnl_link_get_operstate(link);
	char* oper_str = rtnl_link_operstate2str(oper, print_buff, PRINT_BUFF_SIZE);
	printf("oper: %s\n", oper_str);
}

void print_addr(struct nl_addr* addr){
	printf("%s\n", nl_addr2str(addr, print_buff, PRINT_BUFF_SIZE));
}

struct nl_sock* get_route_socket(){
	struct nl_sock *sk;
	sk = nl_socket_alloc();
	nl_connect(sk, NETLINK_ROUTE);
	return sk;
}

void default_veth_up(struct rtnl_link* link){
	rtnl_link_set_flags(link, IFF_BROADCAST | IFF_MULTICAST | IFF_UP);
	rtnl_link_set_operstate(link, IF_OPER_LOWERLAYERDOWN);
	rtnl_link_change(get_route_socket(),
	 								link,
									link,
									0);
}

void default_bridge_up(struct rtnl_link* link){
	rtnl_link_set_flags(link, IFF_BROADCAST | IFF_MULTICAST | IFF_UP | IFF_RUNNING);
	rtnl_link_set_operstate(link, IF_OPER_UP);
	rtnl_link_change(get_route_socket(), link,
									link,
									0);
}



char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}
void default_addr_set(struct rtnl_link* link, const char* addr){//(struct rtnl_link* link){
	/*
	struct nl_addr** addr = malloc(sizeof(void*));
	nl_addr_parse("10.1.1.1/24", AF_INET, addr);
	rtnl_link_set_addr(link, *addr);

	rtnl_link_change(get_route_socket(), link,
									link,
									0);
	*/

	/*
	char* command = malloc(1000);
	//ip addr add 10.1.1.1/24 dev ya1
	strcpy(command, "ip addr add ");
	strcpy(command, addr);
	strcpy(command, " dev ");
	strcpy(command, rtnl_link_get_name(link));
	system(command);
	*/
	char* link_name = rtnl_link_get_name(link);
	if (link_name == NULL)
		printf("alarm\n");
	sleep(3);
	printf("%s\n", link_name);
	sleep(3);
	char* res = concat(4,"ip addr add ", addr, " dev ", link_name);
	system(res);
}

void default_addr_set2(struct rtnl_link* link, const char* addr_str){
	struct nl_addr** local_addr = malloc(sizeof(void*));
	int parse = nl_addr_parse(addr_str, AF_INET, local_addr);

	struct rtnl_addr* rtnl_addr = rtnl_addr_alloc();
	int ifin = rtnl_link_get_ifindex(link);
	int set_ifindex = rtnl_addr_set_ifindex(rtnl_addr, ifin);
	int set_local = rtnl_addr_set_local(rtnl_addr, *local_addr);
	//int set_peer = rtnl_addr_set_peer(rtnl_addr, local_addr);

	int addr_add = rtnl_addr_add(get_route_socket(), rtnl_addr, 0);

	//printf("parse:%d ifin:%d local:%d add:%d\n", parse, set_ifindex, set_local, addr_add);


}














//
