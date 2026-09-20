CC = gcc
CCFLAGS = -Werror -Wall -std=c99

BIN_DIR = bin
OBJ_DIR = obj

ALLOCATOR_OBJS = $(OBJ_DIR)/allocator.o $(OBJ_DIR)/memlib.o $(OBJ_DIR)/alloc_list.o
ALLOCATOR_BIN = $(BIN_DIR)/allocator

all: directories $(ALLOCATOR_BIN)

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

$(ALLOCATOR_BIN): $(ALLOCATOR_BIN)
	$(CC) $(CCFLAGS) -o $@ $(FIRSTFIT_OBJS) 

$(OBJ_DIR)/%.o: %.c | directories
	$(CC) $(CCFLAGS) -o $@ -c $<

.PHONY: directories all clean