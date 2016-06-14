EXEC = ../bin/plugind

CC = gcc
#
# basic flags setting
#
CFLAGS += -g -fPIC $(shell pkg-config --cflags glib-2.0) -I../include
LDFLAGS +=  -ldl -lpthread $(shell pkg-config --libs glib-2.0) -lmicrohttpd

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

#
# object files
#
OBJS = main.o 

#
# directives
#

all : $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(EXEC)

clean:
	rm -f core *.o $(EXEC)

