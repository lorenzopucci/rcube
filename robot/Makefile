# Copyright (c) 2023 Lorenzo Pucci
# You may use, distribute and modify this code under the terms of the MIT
# license.
#
# You should have received a copy of the MIT license along with this program. If
# not, see: <https://mit-license.org>.

SRC_DIR := ../src
INCLUDE_DIR := ../include
BIN_DIR := bin
LIB_BIN_DIR := ../bin
UI_DIR := ../ui
BOT_DIR := .

CXX := g++
CC := gcc
CFLAGS := -Wall -I$(INCLUDE_DIR)

ifeq ($(DEBUG),true)
	CFLAGS += -g
endif

UI_TARGET := $(LIB_BIN_DIR)/ui.o
BOT_TARGET := $(LIB_BIN_DIR)/robot.o

FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(LIB_BIN_DIR)/%.o,$(FILES))

UI_FILES = $(wildcard $(UI_DIR)/*.cpp)
UI_OBJECTS = $(patsubst $(UI_DIR)/%.cpp,$(LIB_BIN_DIR)/ui_%.o,$(UI_FILES))

BOT_FILES = $(wildcard $(BOT_DIR)/*.cpp)
BOT_C_FILES = $(wildcard $(BOT_DIR)/*.c)
BOT_OBJECTS = $(patsubst $(BOT_DIR)/%.cpp,$(BIN_DIR)/bot_%.o,$(BOT_FILES)) \
	$(patsubst $(BOT_DIR)/%.c,$(BIN_DIR)/bot_%.o,$(BOT_C_FILES))

RCUBE_CFLAGS :=
RCUBE_LD_FLAGS :=

ifneq ($(LUA),false)
	RCUBE_LD_LIBS := lua
	RCUBE_CFLAGS := $(shell pkg-config --cflags $(RCUBE_LD_LIBS))
	RCUBE_LD_FLAGS := $(shell pkg-config --libs $(RCUBE_LD_LIBS))
	CFLAGS += $(RCUBE_CFLAGS)
	
else
	CFLAGS += -DIGNORE_LUA
endif

# only used to compile the UI
UI_LD_LIBS = glew glfw3 glm
UI_CFLAGS = $(shell pkg-config --cflags $(UI_LD_LIBS)) $(RCUBE_CFLAGS)
UI_LD_FLAGS = $(shell pkg-config --libs $(UI_LD_LIBS)) $(RCUBE_LD_FLAGS)

# only used to compile the robot controller
BOT_LD_LIBS = opencv
BOT_CFLAGS = $(shell pkg-config --cflags $(BOT_LD_LIBS)) $(RCUBE_CFLAGS) \
	$(UI_CFLAGS) -I/usr/local/include
BOT_LD_FLAGS = $(shell pkg-config --libs $(BOT_LD_LIBS)) $(RCUBE_LD_FLAGS) \
	$(UI_LD_FLAGS) -lraspicam -lraspicam_cv -lwiringPi -lpthread

# create directories if they don't exist
$(shell if [ ! -d "${BIN_DIR}" ]; then mkdir -p ${BIN_DIR}; fi;)


$(BOT_TARGET): $(OBJECTS) $(filter-out ../bin/ui_main.o, $(UI_OBJECTS)) \
	$(BOT_OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ $(BOT_CFLAGS) $(BOT_LD_FLAGS)


# rule valid for each cpp file
$(BIN_DIR)/bot_%.o: $(BOT_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) $^ -o $@

# rule valid for each c file
$(BIN_DIR)/bot_%.o: $(BOT_DIR)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@


.PHONY: clean clean-all robot

clean:
	rm $(BIN_DIR)/*

clean-all: clean

robot: $(BOT_TARGET)