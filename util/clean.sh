#!/bin/sh

ip link del mybr
ip netns delete ns1
ip netns delete ns2

ip link del myveth1
ip link del myveth_ns1

ip link del myveth1
ip link del myveth_ns1
