BINARY=bin/pwgen
OBJECTS=obj/pwgen.o

CC=clang
CFLAGS=-std=c99 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: all, clean

all: $(BINARY)
	@echo [ Strip ] $<
	@strip $(BINARY)

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@echo [Linking] $@
	@$(CC) $(OBJECTS) -o $(BINARY)

obj/%.o: src/%.c
	@mkdir -p obj
	@echo [Compile] $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo [ Clean ] bin
	@rm -rf bin
	@echo [ Clean ] obj
	@rm -rf obj

