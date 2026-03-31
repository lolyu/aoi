# Makefile for vpp_list_interfaces demo
#
# Prerequisites (Debian/Ubuntu):
#   sudo apt-get install vpp vpp-dev vpp-plugin-core
#
# Build:
#   make
#
# Run (VPP must already be running):
#   sudo ./vpp_list_interfaces

CC      ?= gcc
CXX     ?= g++

# VPP installs its headers under /usr/include by default.
# Plugin headers (linux_cp, acl, vxlan, ...) go under /usr/include/vpp_plugins.
CFLAGS  := -g -O0 \
           -I/usr/include \
           -I/usr/include/vpp_plugins

CXXFLAGS := $(CFLAGS)

# Libraries required by the socket-based VPP client
LIBS    := -lvapiclient     \
           -lvppapiclient   \
           -lvlibapi        \
           -lvlibmemoryclient \
           -lsvm            \
           -lvppinfra       \
           -lvlib           \
           -lvatplugin      \
           -lpthread

.PHONY: all clean

all: vpp_list_interfaces vpp_client_connect

vpp_list_interfaces: vpp_list_interfaces.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

vpp_client_connect: vpp_client_connect.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f vpp_list_interfaces vpp_client_connect
