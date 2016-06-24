#
# Copyright (C) 2016 Red Hat, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Author: Todd Gill <tgill@redhat.com>
#

PLUGINDIR = ../bin
PLUGIN = $(PLUGINDIR)/libmpath.so.0.0

CC = gcc
SYSTEMD_INCLUDES = $(shell pkg-config --cflags libsystemd glib-2.0)
SYSTEMD_LDFLAGS = $(shell pkg-config --libs libsystemd glib-2.0) 
CFLAGS += -g -fPIC $(SYSTEMD_INCLUDES) -I./ -I../../include
LDFLAGS +=  -ldl $(SYSTEMD_LDFLAGS) 

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

#
# object files
#
OBJS = multipath_plugin.o multipath_dbus.o
LDFLAGS += -lpthread -ldmmp

#
# directives
#
all : $(PLUGIN)

$(PLUGIN): $(OBJS)
	mkdir  -p $(PLUGINDIR)
	gcc -g -shared -Wl,-soname,$(PLUGIN) \
		-o $(PLUGIN) $(OBJS) $(LDFLAGS) 

clean:
	rm -f core *.o $(PLUGIN)

