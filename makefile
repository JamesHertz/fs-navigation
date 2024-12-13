SRC_DIR := src
BIN_DIR := obj
BIN_DIR := bin

# O_FILE := $(SRC_DIR)/commands.h 
CFLAGS := -Wall -O2

EXE  := $(BIN_DIR)/main
SRC  := $(wildcard $(SRC_DIR)/*.c) # takes all the file .h
OBJ  := $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

TEST_LD  := -lcriterion
TEST_EXE := $(BIN_DIR)/tests
HEADERS  := $(wildcard $(SRC_DIR)/*.h)

EXE_OBJ  := $(filter-out $(TEST_EXE).o, $(OBJ))
TEST_OBJ := $(filter-out $(EXE).o, $(OBJ))

# used for installing purpose
FS_SCRIPT_FILE := $(SRC_DIR)/main.sh

.PHONY: clean install

all: $(EXE)

$(EXE): $(EXE_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(EXE_OBJ)

$(TEST_EXE): $(TEST_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(TEST_LD)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

test: $(TEST_EXE)
	$(TEST_EXE)

install: $(EXE) $(FS_SCRIPT_FILE)
	@echo $(FS_SCRIPT_FILE)
	@echo $(EXE)

debug:
	@echo $(TEST_OBJ)
	@echo $(EXE_OBJ)

clean:
	@$(RM) -rfv $(BIN_DIR)
