BINARY=bin/pwgen
OBJECTS=obj/pwgen.o
HEADERS=src/messages.h

CC=clang
CFLAGS=-std=c99 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: all, clean

all: $(BINARY)

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@echo [Linking] $@
	@$(CC) $(OBJECTS) -o $(BINARY)
	@echo [ Strip ] $<
	@strip $(BINARY)

obj/%.o: src/%.c $(HEADERS)
	@mkdir -p obj
	@echo [Compile] $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo [ Clean ] bin
	@rm -rf bin
	@echo [ Clean ] obj
	@rm -rf obj

