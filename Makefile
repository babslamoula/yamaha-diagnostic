CC      = gcc
WARN    = -Wall -Wextra -pedantic -Wshadow -std=c99
CFLAGS  = $(WARN) -O2 -Iinclude -MMD -MP
LDFLAGS =

# Build debug : symboles + ASAN/UBSAN actifs
#   make debug
DEBUG_FLAGS = $(WARN) -O0 -g -Iinclude -MMD -MP \
              -fsanitize=address -fsanitize=undefined

SRC_DIR = src
OBJ_DIR = obj
BIN     = yamaha_diag

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean debug test help

all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Inclure les .d generes par -MMD pour reconstruire quand un header change
-include $(DEPS)

debug: clean
	$(MAKE) CFLAGS="$(DEBUG_FLAGS)" LDFLAGS="-fsanitize=address -fsanitize=undefined"

# Tests fumee : verifient les 3 bugs P0 (EOF, cycle, MT-07 cliquetis)
test: all
	@bash tests/smoke.sh

clean:
	rm -rf $(OBJ_DIR) $(BIN)

help:
	@echo "Cibles : all (defaut), clean, debug, test"
