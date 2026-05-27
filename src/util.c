/* util.c - implementation des helpers partages */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

#ifdef _WIN32
#  include <windows.h>
#endif

void vider_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

int lire_oui_non(const char *invite)
{
    char rep[16];

    for (;;) {
        if (invite != NULL)
            printf("%s", invite);

        if (scanf("%15s", rep) != 1) {
            /* EOF ou erreur de lecture : on ne re-invite pas indefiniment,
             * on rend la main au caller qui decidera quoi faire. */
            vider_stdin();
            return -1;
        }
        vider_stdin();

        if (strcmp(rep, "oui") == 0 || strcmp(rep, "OUI") == 0 ||
            strcmp(rep, "o") == 0   || strcmp(rep, "O") == 0   ||
            strcmp(rep, "y") == 0   || strcmp(rep, "Y") == 0   ||
            strcmp(rep, "yes") == 0)
            return 1;

        if (strcmp(rep, "non") == 0 || strcmp(rep, "NON") == 0 ||
            strcmp(rep, "n") == 0   || strcmp(rep, "N") == 0   ||
            strcmp(rep, "no") == 0)
            return 0;

        printf("  [?] Reponse non reconnue. Tapez 'oui' ou 'non'.\n");
    }
}

char *util_trim(char *s)
{
    char *end;

    if (s == NULL)
        return NULL;

    while (isspace((unsigned char)*s))
        s++;

    if (*s == '\0')
        return s;

    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';
    return s;
}

void util_init_console(void)
{
#ifdef _WIN32
    /* La console Windows par defaut est cp437/cp1252 et ne sait pas afficher
     * les caracteres box-drawing UTF-8 utilises dans interface.c. On force
     * la page de code de sortie en UTF-8. */
    SetConsoleOutputCP(CP_UTF8);
#endif
}
