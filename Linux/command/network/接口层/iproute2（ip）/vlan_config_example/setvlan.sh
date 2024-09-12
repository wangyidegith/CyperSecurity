#!/bin/bash
sudo ip link add link enp0s3 name enp0s3.123 type vlan id 123
sudo ip addr add 192.168.34.88/24 dev enp0s3.123
sudo ip addr del 10.0.34.88/24 dev enp0s3
sudo ip link set enp0s3.123 up
exit 0
