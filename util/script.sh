#!/bin/sh

ip netns add ns1
ip netns add ns2

ip link add myveth1 type veth peer name myveth_ns1
ip link set myveth_ns1 netns ns1
ip addr add 10.1.0.2/16 dev myveth1
ip netns exec ns1 ip addr add 10.0.0.2/16 dev myveth_ns1
ip link set myveth1 up
ip netns exec ns1 ip link set myveth_ns1 up


ip link add myveth2 type veth peer name myveth_ns2
ip link set myveth_ns2 netns ns2
ip addr add 10.1.0.3/16 dev myveth2
ip netns exec ns2 ip addr add 10.0.0.3/16 dev myveth_ns2
ip link set myveth2 up
ip netns exec ns2 ip link set myveth_ns2 up

ip link add name mybr type bridge
ip addr add 10.0.0.1/16 dev mybr
ip link set mybr up

ip link set myveth1 master mybr
ip link set myveth2 master mybr
