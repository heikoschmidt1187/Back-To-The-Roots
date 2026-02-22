CC := gcc

TARGET := back_to_the_roots
SRCS := back_to_the_roots.c hash/md4/md4.c
OBJS := $(SRCS:.c=.o)

# Release flags (dein bisheriges Verhalten)
CFLAGS_RELEASE := -Wall -Wextra -O2

# Debug flags (für VSCode/GDB)
CFLAGS_DEBUG := -Wall -Wextra -O0 -g3 -ggdb \
	-fno-omit-frame-pointer -fno-inline \
	-DDEBUG

# Optional: Sanitizer (super hilfreich beim Debuggen)
SANITIZERS := -fsanitize=address,undefined
# Wenn du Sanitizer nicht willst: SANITIZERS :=

LDFLAGS_DEBUG := $(SANITIZERS)
CFLAGS_DEBUG  += $(SANITIZERS)

.PHONY: all release debug clean

all: release

release: CFLAGS := $(CFLAGS_RELEASE)
release: $(TARGET)

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

back_to_the_roots.o: hash/md4/md4.h

clean:
	rm -f $(TARGET) $(OBJS)