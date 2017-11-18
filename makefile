BINARY=bin/pwgen
OBJECTS=obj/pwgen.o
HEADERS=src/messages.h

CC=clang
CFLAGS=-std=c99 							\
		-Wall -Wextra -Werror -pedantic 	\
		-O2

BLD=`tput bold`
RED=`tput setaf 1`
GRN=`tput setaf 2`
YLW=`tput setaf 3`
BLU=`tput setaf 5`
TEL=`tput setaf 6`
NRM=`tput sgr0`

.PHONY: all, clean

all: $(BINARY)

loc:
	@printf "%s[ Lines ]%s " "$(BLD)$(YLW)" $(NRM)
	@cat src/*| wc -l

$(BINARY): $(OBJECTS)
	@mkdir -p bin
	@printf "%s[Linking]%s $@\n" "$(BLD)$(TEL)" $(NRM)
	@$(CC) $(OBJECTS) -o $@
	@printf "%s[ Strip ]%s $@\n" "$(BLD)$(TEL)" $(NRM)
	@strip $@
	@printf "%s[Success] Build Succeeded!%s\n" "$(BLD)$(GRN)" $(NRM)

obj/%.o: src/%.c $(HEADERS)
	@mkdir -p obj
	@printf "%s[Compile]%s $<\n" "$(BLD)$(BLU)" $(NRM)
	@$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	@printf "%s[ Clean ]%s bin\n" "$(BLD)$(RED)" $(NRM)
	@rm -rf bin
	@printf "%s[ Clean ]%s obj\n" "$(BLD)$(RED)" $(NRM)
	@rm -rf obj
