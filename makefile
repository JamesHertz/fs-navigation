SRC_DIR := src
BIN_DIR := obj
BIN_DIR := bin

O_FILE := $(SRC_DIR)/commands.h 
CFLAGS := -Wall -O2

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.h) # takes all the file .h
OBJ := $(SRC:$(SRC_DIR)/%.h=$(BIN_DIR)/%.o) $(SRC_DIR)/main.c # turns the texts .h to to .o

# used for installing purpose
FS_SCRIPT_FILE := $(SRC_DIR)/main.sh

.PHONY: clean install

all: $(EXE)

$(EXE): $(OBJ) $(O_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

install: $(EXE) $(FS_SCRIPT_FILE)
	@echo $(FS_SCRIPT_FILE)
	@echo $(EXE)

clean:
	@$(RM) -rfv $(BIN_DIR)
