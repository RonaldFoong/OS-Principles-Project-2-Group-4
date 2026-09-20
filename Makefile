CC = gcc
CCFLAGS = -Werror -Wall -std=c99

BIN_DIR = bin
OBJ_DIR = obj

FIRSTFIT_OBJS = $(OBJ_DIR)/firstfit.o $(OBJ_DIR)/memlib.o
BESTFIT_OBJS = $(OBJ_DIR)/bestfit.o $(OBJ_DIR)/memlib.o
QUICKFIT_OBJS = $(OBJ_DIR)/quickfit.o $(OBJ_DIR)/memlib.o

FIRSTFIT_BIN = $(BIN_DIR)/firstfit
BESTFIT_BIN = $(BIN_DIR)/bestfit
QUICKFIT_BIN = $(BIN_DIR)/quickfit

all: directories $(FIRSTFIT_BIN) $(BESTFIT_BIN) $(QUICKFIT_BIN)

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

$(FIRSTFIT_BIN): $(FIRSTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(FIRSTFIT_OBJS) 

$(BESTFIT_BIN): $(BESTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(BESTFIT_OBJS)

$(QUICKFIT_BIN): $(QUICKFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(QUICKFIT_OBJS)

$(OBJ_DIR)/%.o: %.c | directories
	$(CC) $(CCFLAGS) -o $@ -c $<

.PHONY: directories all clean