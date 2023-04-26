CC = gcc

APP_NAME = geometry
LIB_NAME = libgeometry
TEST_NAME = test 

CFLAGS = -Wall -Werror
CPPFLAGS = -I src -MD -MMD

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test 

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

TEST_SOURCE = $(shell find $(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJECTS = $(TEST_SOURCE:$(TEST_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(TEST_DIR)/%.o)


DEPS = $(APP_OBJECTS:.o=.h) $(LIB_OBJECTS:.o=.h)

all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ -lm

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: run

run:
	./bin/geometry

.PHONY: clean

clean:
	$(RM) $(APP_PATH) $(OBJ_DIR)/*/*/*.[oad]
	$(RM) $(BIN_DIR)/*.[exed]
	$(RM) obj/test/*.[od]

.PHONY: test

test: $(TEST_PATH)

$(TEST_PATH): $(TEST_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ -lm