CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Isrc/intf
SRC_DIR := src
BUILD_DIR := build/obj
TARGET := app

SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: 
	$(CC) $(CFLAGS) -c $(shell find $(SRC_DIR) -name '$*.c') -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(TARGET)

.PHONY: all clean

