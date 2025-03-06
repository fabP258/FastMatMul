SRC_DIR = src
LIB_DIR = src/lib
TEST_DIR = test
BUILD_DIR = build

CC = gcc
CFLAGS = -O3 -I $(LIB_DIR)

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
LIB_FILES = $(wildcard $(LIB_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_FILES))
TEST_OBJ_FILES = $(TEST_FILES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = matmul

all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ_FILES) $(LIB_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Build object files from src
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files from lib
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	mkdir -p $(BUILD_DIR)/lib
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files from test
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJ_FILES) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o test $^
	./test

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
