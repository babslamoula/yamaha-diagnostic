yamaha_diag : obj/main.o obj/base_faits.o obj/base_regles.o obj/fichiers.o obj/interface.o obj/moteur.o obj/solutions.o obj/structures.o obj/util.o
	gcc obj/main.o obj/base_faits.o obj/base_regles.o obj/fichiers.o obj/interface.o obj/moteur.o obj/solutions.o obj/structures.o obj/util.o -o yamaha_diag -Wall -Wextra -pedantic -Wshadow -std=c99 -O2

obj :
	mkdir -p obj

obj/main.o : src/main.c include/base_faits.h include/base_regles.h include/fichiers.h include/interface.h include/moteur.h include/solutions.h include/structures.h include/util.h | obj
	gcc -c src/main.c -o obj/main.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/base_faits.o : src/base_faits.c include/base_faits.h include/structures.h include/util.h | obj
	gcc -c src/base_faits.c -o obj/base_faits.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/base_regles.o : src/base_regles.c include/base_regles.h include/structures.h include/util.h | obj
	gcc -c src/base_regles.c -o obj/base_regles.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/fichiers.o : src/fichiers.c include/fichiers.h include/base_faits.h include/base_regles.h include/structures.h include/util.h | obj
	gcc -c src/fichiers.c -o obj/fichiers.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/interface.o : src/interface.c include/interface.h include/base_faits.h include/base_regles.h include/fichiers.h include/moteur.h include/solutions.h include/structures.h include/util.h | obj
	gcc -c src/interface.c -o obj/interface.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/moteur.o : src/moteur.c include/moteur.h include/base_faits.h include/base_regles.h include/structures.h include/util.h | obj
	gcc -c src/moteur.c -o obj/moteur.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/solutions.o : src/solutions.c include/solutions.h include/structures.h include/util.h | obj
	gcc -c src/solutions.c -o obj/solutions.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/structures.o : src/structures.c include/structures.h include/util.h | obj
	gcc -c src/structures.c -o obj/structures.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

obj/util.o : src/util.c include/util.h | obj
	gcc -c src/util.c -o obj/util.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O2 -Iinclude

debug : clean obj
	gcc -c src/main.c -o obj/main.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/base_faits.c -o obj/base_faits.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/base_regles.c -o obj/base_regles.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/fichiers.c -o obj/fichiers.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/interface.c -o obj/interface.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/moteur.c -o obj/moteur.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/solutions.c -o obj/solutions.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/structures.c -o obj/structures.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc -c src/util.c -o obj/util.o -Wall -Wextra -pedantic -Wshadow -std=c99 -O0 -g -Iinclude -fsanitize=address -fsanitize=undefined
	gcc obj/main.o obj/base_faits.o obj/base_regles.o obj/fichiers.o obj/interface.o obj/moteur.o obj/solutions.o obj/structures.o obj/util.o -o yamaha_diag -Wall -Wextra -pedantic -Wshadow -std=c99 -fsanitize=address -fsanitize=undefined

test : yamaha_diag
	bash tests/smoke.sh
	make -C tests PROJECT_DIR="$(CURDIR)" clean run

docs :
	doxygen Doxyfile

clean-docs :
	rm -rf docs/html

clean :
	rm -rf obj yamaha_diag

help :
	@echo "Cibles : yamaha_diag, clean, debug, test, docs, clean-docs"
