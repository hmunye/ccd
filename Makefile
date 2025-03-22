BINARY = ccd
SRCDIRS = . ./lib
INCDIRS = . ./include

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wconversion

INCLUDES = $(foreach DIR,$(INCDIRS),-I$(DIR))
DEPFLAGS = -MP -MD
DFLAGS = -DDEBUG -g

CFLAGS += $(INCLUDES) $(DEPFLAGS)

CFILES = $(foreach DIR,$(SRCDIRS),$(wildcard $(DIR)/*.c))
OBJFILES = $(patsubst %.c, %.o, $(CFILES)) 
DEPFILES = $(patsubst %.c, %.d, $(CFILES)) 

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
