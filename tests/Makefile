# Makefile pour les tests unitaires de yamaha_diag
#
# Usage :
#   make           - compile tous les tests dans ./bin/
#   make run       - compile + execute tous les tests
#   make clean     - supprime ./bin/
#
# Surchargeable : PROJECT_DIR (par defaut le repertoire frere
# "yamaha-diagnostic-main 2") et CC (par defaut gcc).
#
# Note : tout est mis dans les recipes (commandes shell) avec un quoting
# explicite "...", car les espaces dans PROJECT_DIR cassent l'expansion
# make des listes de fichiers.

PROJECT_DIR ?= ../yamaha-diagnostic-main 2
CC          ?= gcc
BIN_DIR      = bin
WARN         = -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g

TESTS = test_structures   \
        test_base_faits   \
        test_base_regles  \
        test_fichiers     \
        test_moteur       \
        test_solutions    \
        test_interface

BINS  = $(addprefix $(BIN_DIR)/, $(TESTS))

.PHONY: all run clean help

all: $(BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Une regle generique : compile X.c + tous les modules du projet -> bin/X
# Tout est mis en literal shell entre guillemets pour gerer les espaces
# dans PROJECT_DIR.
$(BIN_DIR)/%: %.c test_support.h | $(BIN_DIR)
	@echo "  CC    $@"
	@$(CC) $(WARN) -I"$(PROJECT_DIR)/include" \
	    "$<" \
	    "$(PROJECT_DIR)/src/base_faits.c"  \
	    "$(PROJECT_DIR)/src/base_regles.c" \
	    "$(PROJECT_DIR)/src/fichiers.c"    \
	    "$(PROJECT_DIR)/src/interface.c"   \
	    "$(PROJECT_DIR)/src/moteur.c"      \
	    "$(PROJECT_DIR)/src/solutions.c"   \
	    "$(PROJECT_DIR)/src/structures.c"  \
	    "$(PROJECT_DIR)/src/util.c"        \
	    -o "$@"

# Execute tous les tests depuis la racine du projet (pour que les chemins
# relatifs "data/yamaha_*.txt" et "data/test_*.txt" fonctionnent).
run: all
	@echo "==== Execution des tests unitaires ===="
	@TESTS_ABS=""; for t in $(TESTS); do \
	    TESTS_ABS="$$TESTS_ABS $$(pwd)/$(BIN_DIR)/$$t"; \
	done; \
	pass=0; fail=0; failed=""; \
	for t in $$TESTS_ABS; do \
	    name=$$(basename "$$t"); \
	    if (cd "$(PROJECT_DIR)" && "$$t"); then \
	        pass=$$((pass+1)); \
	    else \
	        fail=$$((fail+1)); \
	        failed="$$failed $$name"; \
	    fi; \
	done; \
	echo ""; \
	echo "==== Resultat : $$pass pass / $$fail fail ===="; \
	if [ $$fail -ne 0 ]; then \
	    echo "  Echecs :$$failed"; \
	    exit 1; \
	fi

clean:
	rm -rf $(BIN_DIR)

help:
	@echo "Cibles : all (defaut), run, clean, help"
	@echo "Variables : PROJECT_DIR (=$(PROJECT_DIR)), CC (=$(CC))"
