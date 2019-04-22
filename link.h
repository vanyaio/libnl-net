#ifndef LINK_H
#define LINK_H

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

#include <stdlib.h>
#include <stdio.h>

/*
#include <netlink/netlink.h>
#include <netlink/cache.h>
#include <netlink/route/link.h>
#include <netlink/socket.h>

#include <netlink/route/link/veth.h>

#include <netlink/types.h>
*/

#include <sys/ioctl.h>
#include <net/if.h>
/*
enum {
	IF_OPER_UNKNOWN,
	IF_OPER_NOTPRESENT,
	IF_OPER_DOWN,
	IF_OPER_LOWERLAYERDOWN,
	IF_OPER_TESTING,
	IF_OPER_DORMANT,
	IF_OPER_UP,
};


extern struct rtnl_link* my_link_get_by_name(const char* name);
extern void print_flags(struct rtnl_link* link);
extern void print_oper(struct rtnl_link* link);
extern void print_addr(struct nl_addr* addr);
extern struct nl_sock* get_route_socket();
extern void default_veth_up(struct rtnl_link* link);
extern void default_bridge_up(struct rtnl_link* link);
extern void default_addr_set(struct rtnl_link* link, const char* addr);
extern char* concat(int count, ...);
*/
#endif
