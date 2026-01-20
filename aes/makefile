CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
INCLUDE = -I./include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/aes_tool

SRCS = src/main.c src/aes.c
OBJS = $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
