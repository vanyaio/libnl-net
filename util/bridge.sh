#!/bin/sh

ip link add name mybr1 type bridge
ip addr add 10.1.0.1/16 dev mybr1
ip link set mybr1 up
