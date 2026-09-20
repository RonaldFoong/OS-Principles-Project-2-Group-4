CC = gcc
CCFLAGS = -Werror -Wall -std=c99

BIN_DIR = bin
OBJ_DIR = obj

FIRSTFIT_OBJS = $(OJB_DIR)/firstfit.o $(OJB_DIR)/memlib.o
BESTFIT_OBJS = $(OJB_DIR)/bestfit.o $(OJB_DIR)/memlib.o
QUICKFIT_OBJS = $(OJB_DIR)/quickfit.o $(OJB_DIR)/memlib.o

FIRSTFIT_BIN = $(BIN_DIR)/firstfit
BESTFIT_BIN = $(BIN_DIR)/bestfit
QUICKFIT_BIN = $(BIN_DIR)/quickfit

all: directories bin/firstfit.o bin/bestfit.o bin/quickfit.o

directories: bin obj
	mkdir -p bin obj

clean:
	rm -rf bin obj

$(FIRSTFIT_BIN): directories $(FIRSTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(FIRSTFIT_OBJS) 

$(BESTFIT_BIN): directories $(BESTFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(BESTFIT_OBJS)

$(QUICKFIT_BIN): directories $(QUICKFIT_OBJS)
	$(CC) $(CCFLAGS) -o $@ $(QUICKFIT_OBJS)

$(OBJ_DIR)/%.o: %.c | directories
	$(CC) $(CCFLAGS) -o $@ -c $<

.PHONY: directories all clean