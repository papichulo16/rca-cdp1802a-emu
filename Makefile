# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude

# Directories
SRC_DIR := src
BUILD_DIR := build/obj/

# Target binary name
TARGET := app

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Compile source files into build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

