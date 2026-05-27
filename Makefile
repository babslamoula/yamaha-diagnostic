yamaha_diag : main.o structures.o base_regles.o base_faits.o moteur.o fichiers.o interface.o solutions.o
	gcc main.o structures.o base_regles.o base_faits.o moteur.o fichiers.o interface.o solutions.o -o yamaha_diag -Wall -Wextra -pedantic -std=c99

main.o : src/main.c include/structures.h include/base_regles.h include/base_faits.h include/moteur.h include/fichiers.h include/interface.h include/solutions.h
	gcc -c src/main.c -o main.o -Wall -Wextra -pedantic -std=c99 -Iinclude

structures.o : src/structures.c include/structures.h
	gcc -c src/structures.c -o structures.o -Wall -Wextra -pedantic -std=c99 -Iinclude

base_regles.o : src/base_regles.c include/base_regles.h include/structures.h
	gcc -c src/base_regles.c -o base_regles.o -Wall -Wextra -pedantic -std=c99 -Iinclude

base_faits.o : src/base_faits.c include/base_faits.h include/structures.h
	gcc -c src/base_faits.c -o base_faits.o -Wall -Wextra -pedantic -std=c99 -Iinclude

moteur.o : src/moteur.c include/moteur.h include/base_faits.h include/solutions.h include/structures.h
	gcc -c src/moteur.c -o moteur.o -Wall -Wextra -pedantic -std=c99 -Iinclude

fichiers.o : src/fichiers.c include/fichiers.h include/base_regles.h include/base_faits.h include/structures.h
	gcc -c src/fichiers.c -o fichiers.o -Wall -Wextra -pedantic -std=c99 -Iinclude

interface.o : src/interface.c include/interface.h
	gcc -c src/interface.c -o interface.o -Wall -Wextra -pedantic -std=c99 -Iinclude

solutions.o : src/solutions.c include/solutions.h include/structures.h
	gcc -c src/solutions.c -o solutions.o -Wall -Wextra -pedantic -std=c99 -Iinclude

clean :
	rm -f *.o yamaha_diag
