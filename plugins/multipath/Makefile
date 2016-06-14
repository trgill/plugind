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

PLUGIN = ../bin/libmpath.so.0.0

CC = gcc
DBUS_INCLUDES = $(shell pkg-config --cflags glib-2.0 gio-2.0\
	gio-unix-2.0 dbus-1)
CFLAGS += -g -fPIC $(DBUS_INCLUDES) -I./ -I../../include
LDFLAGS +=  -ldl -lmpathcmd

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

#
# object files
#
OBJS = multipath_plugin.o dbus_api.o multipath_dbus_generated.o
LDFLAGS += -lpthread $(shell pkg-config --libs glib-2.0 gio-2.0 \
	gio-unix-2.0 dbus-1) -ldmmp
#
# directives
#
all : $(PLUGIN)

$(PLUGIN): multipath_dbus_generated.c $(OBJS)
	gcc -g -shared -Wl,-soname,$(PLUGIN) \
		-o $(PLUGIN) $(OBJS) $(LDFLAGS) 

multipath_dbus_generated.c: multipath_api.xml 
		gdbus-codegen --generate-c-code multipath_dbus_generated \
		--c-namespace multipath --c-generate-object-manager \
		--interface-prefix com.redhat.multipath multipath_api.xml

clean:
	rm -f core *.o $(PLUGIN)

