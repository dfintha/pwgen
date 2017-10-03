BINARY=bin/pwgen
OBJECTS=obj/pwgen.o

CC=clang
CFLAGS=-std=c99 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: all, clean

all: $(BINARY)
	@strip $(BINARY)

$(BINARY): $(OBJECTS)
	mkdir -p bin
	$(CC) $(OBJECTS) -o $(BINARY)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin obj

