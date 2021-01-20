# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -std=c99 -Wall -Werror -Wextra

# name for executable
EXE = main

# space-separated list of header files
HDRS = Board.h

# to make it portable
# LDLIBS=-lsocket

# space-separated list of source files
SRCS = Board.c main.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies
$(OBJS): $(HDRS)

# housekeeping
# or we can use: rm -f core $(EXE) *.o
clean:
	rm -f *.o main

