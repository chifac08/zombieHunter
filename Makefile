#Vars
INCL_DIR=incl
SRC_DIR=src
OBJ_DIR=obj
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INCL=$(wildcard $(INCL_DIR)/*.h)
CC=gcc
EXE=zombieHunterService
CPPFLAGS=-Iincl
CFLAGS=-Wall
LDFLAGS= -Llib

.PHONY: all clean

all: $(EXE)
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
