CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
TARGET = main

SRC_DIRS = . structures repl csv commands
VPATH = $(subst $(, ,),:,$(SRC_DIRS))
CPPFLAGS = $(foreach dir,$(SRC_DIRS),-I$(dir))

SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(patsubst %.c,%.o,$(notdir $(SRCS)))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up generated files..."
	rm -f $(OBJS) $(TARGET)
	@echo "Done."

