CC := gcc
CFLAGS := -Wall -Wextra -O2
TARGET := back_to_the_roots

SRCS := back_to_the_roots.c hash/md4/md4.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

back_to_the_roots.o: hash/md4/md4.h

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
