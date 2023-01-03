SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

O_FILE := $(SRC_DIR)/commands.h 
CFLAGS := -Wall

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.h) # takes all the file .h
OBJ := $(SRC:$(SRC_DIR)/%.h=$(OBJ_DIR)/%.o) $(SRC_DIR)/main.c # turns the texts .h to to .o

.PHONY: clean deploy

all: $(EXE)

$(EXE): $(OBJ) $(O_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rfv $(BIN_DIR) $(OBJ_DIR)

#####################################################
#                  FS INSTALATION:                  #
#####################################################

# TODO: change the two variables below for your situation
FS_BASE_DIR := /opt/fs-nav
RC_FILE     := ~/.zprofile

# other dirs
FS_EXE_DIR  := $(FS_BASE_DIR)/bin
DIRS 		:= $(FS_BASE_DIR) $(FS_EXE_DIR)

# files it will create
FS_EXE_FILE := $(FS_EXE_DIR)/fs-exe
FS_SCRIPT_FILE := $(FS_BASE_DIR)/script.sh

# todo (for me): think about is here below :)
deploy: $(DIRS) $(EXE)
	cp src/main.sh $(FS_SCRIPT_FILE)
	cp bin/main $(FS_EXE_FILE)

install: deploy
	echo "\n export FS_EXE=$(FS_EXE_FILE) " >> $(RC_FILE)
	echo "\n source $(FS_SCRIPT_FILE) " >> $(RC_FILE)

$(FS_BASE_DIR) $(FS_EXE_DIR) :
	mkdir $@

