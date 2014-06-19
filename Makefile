########################################################################
# General Makefile
########################################################################
PROGRAM = example
SRC = example.c \
      params.c
DEPS = params.h

OBJS = $(patsubst %.c, %.o, $(SRC))

CC = gcc

LDFLAGS += -L/opt/local/lib
CFLAGS += -I/opt/local/include

CFLAGS += -Wall
CFLAGS += -ggdb
CFLAGS += -O0
#CFLAGS += -O3

LIBS += -lm

.PHONEY: clean

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROGRAM)