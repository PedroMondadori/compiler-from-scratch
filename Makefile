CC = gcc

DEBUG_INFO_FLAGS = -DPRINT_TAC -DPRINT_AST -DPRINT_HASH
CFLAGS = -Wall -Wextra -Iinclude $(DEBUG_INFO_FLAGS)

YACC = yacc
LEX = flex

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
BIN_DIR = bin
EXAMPLES_BIN_DIR = examples/bin

# List all the C source files
C_FILES := $(wildcard $(SRC_DIR)/*.c)
# Generate corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES))

# The target executable
TARGET = cfs

.PHONY: all clean

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ_FILES) $(BUILD_DIR)/y.tab.o $(BUILD_DIR)/lex.yy.o | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/y.tab.o: $(BUILD_DIR)/y.tab.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.o: $(BUILD_DIR)/lex.yy.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/y.tab.c: $(SRC_DIR)/parser.y | $(BUILD_DIR)
	$(YACC) -d -o $@ $<

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/scanner.l | $(BUILD_DIR)
	$(LEX) -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(EXAMPLES_BIN_DIR)
