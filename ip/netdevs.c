#include "../netdevs.h"

int netdevs_set_devs(int node_cnt, pid_t* node_pids){
  char* bridge_name = netdevs_set_bridge();

  for (int i = 0; i < node_cnt; i++)
    netdevs_set_veth(i, node_pids[i], bridge_name);

  free(bridge_name);
  return 1;
}

int netdevs_set_node(struct node_entry* entry, int num){
  char* name = get_veth_name(num, IN_NS);
  /*
	struct rtnl_link* veth = my_link_get_by_name(name);
	if (veth == NULL)
		printf("null1\n");
	default_veth_up(veth);

	veth = my_link_get_by_name(name);
	if (veth == NULL)
		printf("null2\n");
	default_addr_set(veth, entry->ip_addr);
  */
  char* ns = get_ns_name(num);
  char buff[BUFF_SIZE];
  //up:
  strcpy(buff, "ip link set ");
  strcat(buff, name);
  strcat(buff, " up");
  system(buff);
  //ip addr add 10.1.0.3/16 dev myveth2
  strcpy(buff, "ip addr add ");
  strcat(buff, entry->ip_addr);
  strcat(buff, " dev ");
  strcat(buff, name);
  system(buff);

	system("ip link set lo up");
  return 1;

}

char* netdevs_set_bridge(){
  system("ip link add name mybr type bridge");
	system("ip addr add 10.0.0.1/16 dev mybr");
	system("ip link set mybr up");
  char* bridge_name = malloc(BUFF_SIZE * sizeof(char));
  strcpy(bridge_name, "mybr");
  return bridge_name;
}

int netdevs_set_veth(int num, pid_t pid, char* master_bridge){
  char* name = get_veth_name(num, 0);//weird bug with IN_NS not working
  char* name_ns = get_veth_name(num, 1);
  char* ns = get_ns_name(num);
  char* buff[BUFF_SIZE];
  //ip netns add ns1
  /*
  strcpy(buff, "ip netns add ");
  strcat(buff, ns);
  system(buff);
  */
  //ip link add myveth1 type veth peer name myveth_ns1
  strcpy(buff, "ip link add ");
  strcat(buff, name);
  strcat(buff, " type veth peer name ");
  strcat(buff, name_ns);
  system(buff);
  //ip link set myveth_ns1 netns ns1
  strcpy(buff, "ip link set ");
  strcat(buff, name_ns);
  strcat(buff, " netns ");
  strcat(buff, name);
  system(buff);
  //ip link set myveth1 up
  strcpy(buff, "ip link set ");
  strcat(buff, name);
  strcat(buff, " up");
  system(buff);
  //?addr:

  //ip link set myveth1 master mybr
  strcpy(buff, "ip link set ");
  strcat(buff, name);
  strcat(buff, " master mybr");
  system(buff);
  /*  rtnl_link_veth_add(get_route_socket(), name, name_ns, pid);

  struct rtnl_link* veth = my_link_get_by_name(name);
  if (veth == NULL)
  	printf("null1\n");
  default_veth_up(veth);
  veth = my_link_get_by_name(name);
  if (veth == NULL)
  	printf("null2\n");
  default_addr_set(veth, get_veth_addr(num));

  struct nl_cache *cache;
  struct nl_sock *sk1;
  sk1 = nl_socket_alloc();
  nl_connect(sk1, NETLINK_ROUTE);
  if (rtnl_link_alloc_cache(sk1, AF_UNSPEC, &cache) < 0)
  	printf("error\n");

  int master_index = rtnl_link_name2i(cache, master_bridge);
  if (!master_index)
  	printf("error master ind\n");
  rtnl_link_set_master(veth, master_index);
  rtnl_link_change(get_route_socket(), veth,
  								veth,
  								0);
                  */
}


char* get_veth_name(int i, int in_ns){
	char i_str[BUFF_SIZE];
	sprintf(i_str, "%d", i + 1);

	if (!in_ns)
		return concat(2, "myveth", i_str);

	return concat(2, "myveth_ns", i_str);
}
char* get_veth_addr(int i){
	char i_str[BUFF_SIZE];
	sprintf(i_str, "%d", i + 2);
	return concat(3, "10.1.0.", i_str, "/16");
}
char* get_ns_name(int num){
  char* buff = malloc(BUFF_SIZE);
  char num_str[BUFF_SIZE];
  sprintf(num_str, "%d", num);
  strcpy(buff, "namespace");
  strcat(buff, num_str);
  return buff;
}
