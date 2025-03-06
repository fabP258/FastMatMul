CC = gcc
CFLAGS = -O3

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

TARGET = matmul

all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
	