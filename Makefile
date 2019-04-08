CC=h5pcc

DEBUG=0

export HDF5_CXX=mpicc
export HDF5_CLINKER=mpicc

TARGET=main
DEPS= pattern.h
OBJS=main.o pattern.o
CFLAGS=-std=c11

ifeq ($(DEBUG), 1)
OPTS=-O0 -g
endif

CFLAGS+=$(OPTS)

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJS) *.h5
