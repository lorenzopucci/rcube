# Copyright (c) 2022 Lorenzo Pucci
# You may use, distribute and modify this code under the terms of the MIT
# license.
#
# You should have received a copy of the MIT license along with this program. If
# not, see: <https://mit-license.org>.

SRC_DIR := src
INCLUDE_DIR := include
BIN_DIR := bin
TEST_DIR := test
UI_DIR := ui

CC := g++
CFLAGS := -I$(INCLUDE_DIR) -fpermissive

ifeq ($(DEBUG),true)
	CFLAGS += -g
endif

TEST_TARGET := $(BIN_DIR)/test.o
UI_TARGET := $(BIN_DIR)/ui.o

FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(FILES))

TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/test_%.o,$(TEST_FILES))

UI_FILES := $(wildcard $(UI_DIR)/*.cpp)
UI_OBJECTS := $(patsubst $(UI_DIR)/%.cpp,$(BIN_DIR)/ui_%.o,$(UI_FILES))

RCUBE_CFLAGS :=
RCUBE_LD_FLAGS :=

ifneq ($(LUA),false)
	RCUBE_LD_LIBS := lua
	RCUBE_CFLAGS := $(shell pkg-config --cflags $(RCUBE_LD_LIBS))
	RCUBE_LD_FLAGS := $(shell pkg-config --libs $(RCUBE_LD_LIBS))
	
	CFLAGS += -DCOMPILE_LUA
endif

# only used to compile the UI
UI_LD_LIBS := glew glfw3 glm
UI_CFLAGS := $(shell pkg-config --cflags $(UI_LD_LIBS)) $(RCUBE_CFLAGS)
UI_LD_FLAGS := $(shell pkg-config --libs $(UI_LD_LIBS)) $(RCUBE_LD_FLAGS)

# create directories if they don't exist
$(shell if [ ! -d "${BIN_DIR}" ]; then mkdir -p ${BIN_DIR}; fi;)

$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(RCUBE_CFLAGS) $(RCUBE_LD_FLAGS)

$(UI_TARGET): $(OBJECTS) $(UI_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(UI_CFLAGS) $(UI_LD_FLAGS)


# rule valid for each file in ./src
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -c $^ -o $@

# rule valid for each file in ./test
$(BIN_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -c $^ -o $@

# rule valid for each file in ./ui
$(BIN_DIR)/ui_%.o: $(UI_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -c $^ -o $@


.PHONY: all clean clean-all test ui lib

lib: $(OBJECTS)

clean:
	rm $(BIN_DIR)/*

clean-all: clean

test: $(TEST_TARGET)

ui: $(UI_TARGET)

all: $(OBJECTS) $(TEST_TARGET) $(UI_TARGET)