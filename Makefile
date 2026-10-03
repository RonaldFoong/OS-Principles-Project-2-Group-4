CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Wno-deprecated-declarations

BIN_DIR = bin
OBJ_DIR = obj

SHARED_OBJS = $(OBJ_DIR)/alloc_list.o \
              $(OBJ_DIR)/mem_common.o \
              $(OBJ_DIR)/driver.o

FIRSTFIT_OBJS = $(OBJ_DIR)/firstfit.o \
                $(OBJ_DIR)/firstfit_lib.o \
                $(SHARED_OBJS)

BESTFIT_OBJS = $(OBJ_DIR)/bestfit.o \
               $(OBJ_DIR)/bestfit_lib.o \
               $(SHARED_OBJS)

QUICKFIT_OBJS = $(OBJ_DIR)/quickfit.o \
                $(OBJ_DIR)/quickfit_lib.o \
                $(SHARED_OBJS)

FIRSTFIT_BIN = $(BIN_DIR)/firstfit
BESTFIT_BIN = $(BIN_DIR)/bestfit
QUICKFIT_BIN = $(BIN_DIR)/quickfit

VPATH = utils firstfit bestfit quickfit
HEADERS = $(wildcard utils/*.h */*_lib.h)

all: directories $(FIRSTFIT_BIN) $(BESTFIT_BIN) $(QUICKFIT_BIN)

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

$(FIRSTFIT_BIN): $(FIRSTFIT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BESTFIT_BIN): $(BESTFIT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(QUICKFIT_BIN): $(QUICKFIT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c $(HEADERS) | directories
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: all directories clean
