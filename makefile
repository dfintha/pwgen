BINARY=bin/pwgen
OBJECTS=obj/pwgen.o
HEADERS=src/messages.h

CC=clang
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -O2

.PHONY: all, clean

all: $(BINARY)

loc:
	@printf "[N] "
	@cat src/*| wc -l

run: $(BINARY)
	@$(BINARY)

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@printf "[L] $@\n"
	@$(CC) $(OBJECTS) -o $@
	@printf "[S] $@\n"
	@strip $@
	@printf "[+] Build Succeeded!\n"

obj/%.o: src/%.c $(HEADERS)
	@mkdir -p obj
	@printf "[C] $<\n"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "[R] bin\n"
	@rm -rf bin
	@printf "[R] obj\n"
	@rm -rf obj

