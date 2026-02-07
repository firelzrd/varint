CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -std=c99

TEST_TARGET = test_varint
TEST_SRCS = test_varint.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

.PHONY: all clean test

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c varint.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TEST_OBJS) $(TEST_TARGET)
