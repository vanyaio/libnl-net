#!/bin/sh

ip address show
echo "_________________________________________"
route -n
echo "_________________________________________ bridge:"
bridge link show
echo "_________________________________________"
echo "_________________________________________"
echo "_________________________________________"
ip netns exec ns1 ip address show
echo "_________________________________________"
ip netns exec ns1 route -n
echo "_________________________________________"
ip netns exec ns1 bridge link show
