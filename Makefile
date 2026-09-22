CC = gcc
CCFLAGS = -Wall -Werror -std=c99 -Wno-deprecated-declarations

BIN_DIR = bin
OBJ_DIR = obj

FIRSTFIT_OBJS = $(OBJ_DIR)/firstfit.o $(OBJ_DIR)/firstfit_lib.o $(OBJ_DIR)/alloc_list.o
BESTFIT_OBJS = $(OBJ_DIR)/bestfit.o $(OBJ_DIR)/bestfit_lib.o $(OBJ_DIR)/alloc_list.o
QUICKFIT_OBJS = $(OBJ_DIR)/quickfit.o $(OBJ_DIR)/quickfit_lib.o $(OBJ_DIR)/alloc_list.o

FIRSTFIT_BIN = $(BIN_DIR)/firstfit
BESTFIT_BIN = $(BIN_DIR)/bestfit
QUICKFIT_BIN = $(BIN_DIR)/quickfit

VPATH = utils firstfit bestfit quickfit

all: directories $(FIRSTFIT_BIN) $(BESTFIT_BIN) $(QUICKFIT_BIN)

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

$(FIRSTFIT_BIN): $(FIRSTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(BESTFIT_BIN): $(BESTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(QUICKFIT_BIN): $(QUICKFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c | directories
	$(CC) $(CCFLAGS) -o $@ -c $<

.PHONY: directories all clean