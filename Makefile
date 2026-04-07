CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Isrc/intf -g 
SRC_DIR := src
BUILD_DIR := build/obj
INTF_DIR := src/intf
ASM_DIR := src/cross-asm
TARGET := rca1802emu

FLEX_SRC := $(ASM_DIR)/scanner.flex
FLEX_OUT := $(ASM_DIR)/lex.yy.c
FLEX_HDR := $(ASM_DIR)/scanner.h
FLEX_OBJ := $(BUILD_DIR)/lex.yy.o

SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -name 'lex.yy.c' 2>/dev/null || true)
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

.PHONY: all clean flex-setup

all: flex-setup $(TARGET)

flex-setup:
	cd $(ASM_DIR) && ./flex_setup.sh && cd ../../

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile the flex-generated C file into an object — depends on flex-setup having run
$(FLEX_OBJ): $(FLEX_OUT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(FLEX_OUT) -o $@

$(TARGET): $(OBJS) $(FLEX_OBJ)
	$(CC) $(OBJS) $(FLEX_OBJ) -o $@ -lcyaml -lyaml
	rm -f $(FLEX_OUT) $(FLEX_HDR)

$(BUILD_DIR)/%.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(shell find $(SRC_DIR) -name '$*.c' ! -name 'lex.yy.c') -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(TARGET)
	#cd $(ASM_DIR) && ./flex_cleanup.sh
