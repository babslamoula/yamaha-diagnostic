# Makefile pour les tests unitaires de yamaha_diag

PROJECT_DIR ?= ..
BIN_DIR = bin

TESTS = test_structures test_base_faits test_base_regles test_fichiers test_moteur test_solutions test_interface

all : $(BIN_DIR)/test_structures $(BIN_DIR)/test_base_faits $(BIN_DIR)/test_base_regles $(BIN_DIR)/test_fichiers $(BIN_DIR)/test_moteur $(BIN_DIR)/test_solutions $(BIN_DIR)/test_interface

$(BIN_DIR) :
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/test_structures : test_structures.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_structures.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_structures"

$(BIN_DIR)/test_base_faits : test_base_faits.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_base_faits.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_base_faits"

$(BIN_DIR)/test_base_regles : test_base_regles.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_base_regles.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_base_regles"

$(BIN_DIR)/test_fichiers : test_fichiers.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_fichiers.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_fichiers"

$(BIN_DIR)/test_moteur : test_moteur.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_moteur.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_moteur"

$(BIN_DIR)/test_solutions : test_solutions.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_solutions.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_solutions"

$(BIN_DIR)/test_interface : test_interface.c test_support.h | $(BIN_DIR)
	gcc -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -I"$(PROJECT_DIR)/include" test_interface.c "$(PROJECT_DIR)/src/base_faits.c" "$(PROJECT_DIR)/src/base_regles.c" "$(PROJECT_DIR)/src/fichiers.c" "$(PROJECT_DIR)/src/interface.c" "$(PROJECT_DIR)/src/moteur.c" "$(PROJECT_DIR)/src/solutions.c" "$(PROJECT_DIR)/src/structures.c" "$(PROJECT_DIR)/src/util.c" -o "$(BIN_DIR)/test_interface"

run : all
	@echo "==== Execution des tests unitaires ===="
	@pass=0; fail=0; failed=""; \
	for t in $(TESTS); do \
	    name="$$t"; \
	    bin="$$(pwd)/$(BIN_DIR)/$$t"; \
	    if (cd "$(PROJECT_DIR)" && "$$bin"); then \
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

clean :
	rm -rf $(BIN_DIR)

help :
	@echo "Cibles : all, run, clean, help"
