BINARY = ccd
SRC = main.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wconversion
DEPFLAGS = -MP -MD
DFLAGS = -DDEBUG -g

CFLAGS += $(DEPFLAGS)

OBJFILES = $(patsubst %.c, %.o, $(SRC)) 
DEPFILES = $(patsubst %.c, %.d, $(SRC)) 

.PHONY: all debug clean help

all: $(BINARY)

debug: CFLAGS += $(DFLAGS)
debug: $(BINARY)

$(BINARY): $(OBJFILES)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJFILES) $(DEPFILES)

help:
	@echo "Available targets:"
	@echo "  all      - Build binary (Default target)"
	@echo "  debug    - Build binary with debugging symbols"
	@echo "  clean    - Clean up generated files (binary, object files, dependencies)"
	@echo "  help     - Show help message"

-include $(DEPFILES)
