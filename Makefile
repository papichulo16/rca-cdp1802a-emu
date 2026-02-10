CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Isrc/intf
FLEX    := flex
SRC_DIR := src
BUILD_DIR := build/obj
INTF_DIR := src/intf
TARGET := rca1802emu

FLEX_SRC := src/cross-asm/scanner.flex
FLEX_OUT := $(INTF_DIR)/lex.yy.c
FLEX_HDR := $(INTF_DIR)/scanner.h

# Find regular sources (flex output will be added as prerequisite)
SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -name 'lex.yy.c' 2>/dev/null || true)
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
FLEX_OBJ := $(BUILD_DIR)/lex.yy.o

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Generate flex file in intf directory
$(FLEX_OUT): $(FLEX_SRC) | $(INTF_DIR)
	$(FLEX) --header-file=$(FLEX_HDR) -o $(FLEX_OUT) $(FLEX_SRC)
	mv scanner.h $(FLEX_HDR) 2>/dev/null || true

$(INTF_DIR):
	mkdir -p $(INTF_DIR)

# Compile flex output
$(FLEX_OBJ): $(FLEX_OUT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(FLEX_OUT) -o $@

$(TARGET): $(OBJS) $(FLEX_OBJ)
	$(CC) $(OBJS) $(FLEX_OBJ) -o $@
	rm -f $(FLEX_OUT) $(FLEX_HDR)

$(BUILD_DIR)/%.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(shell find $(SRC_DIR) -name '$*.c' ! -name 'lex.yy.c') -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(TARGET)
	rm -f $(FLEX_OUT) $(FLEX_HDR)

.PHONY: all clean
