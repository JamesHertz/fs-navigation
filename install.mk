#####################################################
#                  FS INSTALATION:                  #
#####################################################

# TODO: change the two variables below for your situation
FS_BASE_DIR :=
RC_FILE     :=

# to upload my local variables, don't worry about it :)
-include .myvars.mk

FS_EXE_DIR  := $(FS_BASE_DIR)/bin
DIRS 		:= $(FS_BASE_DIR) $(FS_EXE_DIR)

# files it will create
FS_EXE_FILE := $(FS_EXE_DIR)/fs-exe
FS_SCRIPT_FILE := $(FS_BASE_DIR)/script.sh

all: install

install: deploy
ifdef RC_FILE
	echo "\nexport FS_EXE=$(FS_EXE_FILE) " >> $(RC_FILE)
	echo "source $(FS_SCRIPT_FILE) " >> $(RC_FILE)
else
$(error RC_FILE is not define. Please read the TODOS.)
endif

deploy: $(DIRS) $(EXE)
ifdef FS_BASE_DIR
	cp src/main.sh $(FS_SCRIPT_FILE)
	cp bin/main $(FS_EXE_FILE)
else
$(error FS_BASE_DIR is not define. Please read the TODOS.)
endif

$(FS_BASE_DIR) $(FS_EXE_DIR) :
	mkdir $@
