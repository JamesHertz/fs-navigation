SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

O_FILE := $(SRC_DIR)/commands.h 
CFLAGS := -Wall

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.h) # takes all the file .h
OBJ := $(SRC:$(SRC_DIR)/%.h=$(OBJ_DIR)/%.o) $(SRC_DIR)/main.c # turns the texts .h to to .o


.PHONY: clean 

all: $(EXE)

$(EXE): $(OBJ) $(O_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rfv $(BIN_DIR) $(OBJ_DIR)
