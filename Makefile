SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/chip8
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

CPPFLAGS := -Iinclude -MMD -MP	# -I is a preprocessor flag, not a compiler flag
CFLAGS   := -Wall				# some warnings about bad code
LDFLAGS  := -Llib				# -L is a linker flag
LDLIBS   := -lm -lncurses -lGL -lGLU -lglut	-lpthread# Left empty if no libs are needed
MACROS   := -g -D DEBUG 


VALGRIND := valgrind
VALGRIND_FLAGS := --leak-check=full
LOG_FILE := log

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(MACROS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

corax:
	$(EXE) -f roms/3-corax+.ch8

flags:
	$(EXE) -f roms/4-flags.ch8

quirks:
	$(EXE) -f roms/5-quirks.ch8

keypad:
	$(EXE) -f roms/6-keypad.ch8

zero:
	$(EXE) -f roms/zero.ch8

trip8:
	$(EXE) -f roms/trip8.ch8

testing:
	$(EXE) -f roms/F555Test.ch8

valgrind: $(EXE)
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(EXE) -f roms/testing.ch8 > $(LOG_FILE) 2>&1

cppcheck:
	cppcheck --enable=all --cppcheck-build-dir=debug src
