########################################################################
# General Makefile
########################################################################
PROGRAM = ptest
OBJS = hashtab.o \
       params.o \
       params_test.o
DEPS = hashtab.h params.h

ODIR = obj/
SDIR = src/

CC = gcc
CXX = g++

LDFLAGS += -L/opt/local/lib
CFLAGS += -I/opt/local/include
CFLAGS += -I.

CFLAGS += -Wall
CFLAGS += -ggdb
CFLAGS += -O0
#CFLAGS += -O3
#LIBS += -lgsl
#LIBS += -lgslcblas
#LIBS += -llapack -lptcblas -latlas
#LIBS += -lnlopt
#LIBS += -lm

.PHONEY: clean

$(PROGRAM): $(patsubst %, $(ODIR)/%, $(OBJS))
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.c $(patsubst %, $(SDIR)/%, $(DEPS))
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(ODIR)/*.o $(PROGRAM)
