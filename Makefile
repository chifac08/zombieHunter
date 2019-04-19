#Vars
INCL_DIR=-I ./incl -I /sclogger_incl
SRC_DIR=src
OBJ_DIR=obj
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CC=gcc
EXE=zombieHunterService
CPPFLAGS=-Iincl
CFLAGS=-Wall -L ./sclogger
LDFLAGS= -Llib 
LIBS=sclogger.a -lpthread

.PHONY: all clean

all: $(EXE)
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -g -O0 -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCL_DIR) -g -O0 -c $< -o $@

clean:
	$(RM) $(OBJ)
