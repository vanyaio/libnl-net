#ifndef DEBUG_H
#define DEBUG_H

void debug(){
	struct rtnl_link* link = my_link_get_by_name("ya1");
	if (link == NULL)
		printf("null\n");
	printf("_____\n");
	print_flags(link);
	print_oper(link);
}

void debug1(){
	int code = rtnl_link_veth_add(get_route_socket(), "ya1", "ya2", getpid());
	printf("adding code: %d\n", code);

	struct rtnl_link* link = my_link_get_by_name("ya1");
	if (link == NULL)
		printf("null\n");

	debug();
	sleep(6);
	default_veth_up(link);

	debug();
	sleep(10);
	link = my_link_get_by_name("ya1");
	//default_addr_set(link);
	debug();
}
void debug2(){

	struct rtnl_link* link = my_link_get_by_name("ya1");
	if (link == NULL)
		printf("null\n");

	struct nl_addr* addr0 = rtnl_link_get_addr(link);
	print_addr(addr0);
	//struct addrinfo ** result = malloc(sizeof(void*));
	//nl_addr_info(addr, result);
	struct nl_addr** addr = malloc(sizeof(void*));
	nl_addr_parse("10.1.1.1/24", AF_INET, addr);
	print_addr(*addr);
}

void debug3(){
	int code = rtnl_link_bridge_add(get_route_socket(), "mybr");
	printf("%d\n", code);
	struct rtnl_link* br = my_link_get_by_name("mybr");
	if (br == NULL)
		printf("null\n");
	default_bridge_up(br);
	//default_addr_set(br);

	code = rtnl_link_veth_add(get_route_socket(), "myveth", "myveth_s", getpid());
	printf("adding code: %d\n", code);
	struct rtnl_link* veth = my_link_get_by_name("myveth");
	if (veth == NULL)
		printf("null\n");
	default_veth_up(veth);
	//default_addr_set(veth);

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

void debug4(){
	int code = rtnl_link_bridge_add(get_route_socket(), "mybr");
	printf("%d\n", code);
	struct rtnl_link* br = my_link_get_by_name("mybr");
	if (br == NULL)
		printf("null\n");
  print_flags(br);
	print_oper(br);
	default_bridge_up(br);
	br = my_link_get_by_name("mybr");
	print_flags(br);
	print_oper(br);
}

/*
void debug5(){
  int code = rtnl_link_veth_add(get_route_socket(), "myveth", "myveth_s", getpid());
	printf("adding code: %d\n", code);
	struct rtnl_link* veth = my_link_get_by_name("myveth");
	if (veth == NULL)
		printf("null1\n");
	default_veth_up(veth);
  veth = my_link_get_by_name("myveth");
	if (veth == NULL)
		printf("null2\n");
  default_addr_set2(veth, "10.1.1.1/24");
}

void debug6(){
  int code = rtnl_link_bridge_add(get_route_socket(), "mybr");
	struct rtnl_link* br = my_link_get_by_name("mybr");
	default_bridge_up(br);
  br = my_link_get_by_name("mybr");

  default_addr_set2(br, "10.10.10.1/24");
}

void debug7() {
	struct rtnl_link* br1 = my_link_get_by_name("mybr1");
	if (br1 == NULL)
		printf("error\n");
	print_bridge(br1);

  int code = rtnl_link_bridge_add(get_route_socket(), "mybr2");
	struct rtnl_link* br2 = my_link_get_by_name("mybr2");

	print_bridge(br2);
	default_addr_set2(br2, "10.0.0.1/16");
	br2 = my_link_get_by_name("mybr2");
	default_bridge_up(br2);
  br2 = my_link_get_by_name("mybr2");
	print_bridge(br2);
}
*/
#endif

















//
