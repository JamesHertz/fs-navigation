SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

H_FILE := $(SRC_DIR)/records.h
CFLAGS := -Wall

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.c) # takes all the file .c
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) # turns the texts .c to to .o

.PHONY: clean 

all: $(EXE)

$(EXE): $(OBJ) $(H_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(H_FILE) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rfv $(BIN_DIR) $(OBJ_DIR)
