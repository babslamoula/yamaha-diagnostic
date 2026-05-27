/* test_support.h - macros partagees pour les tests unitaires
 *
 * Usage :
 *   #include "test_support.h"
 *   ...
 *   int main(void) {
 *       VERIFIER(2 + 2 == 4);
 *       TERMINER_TEST("mon_test");
 *   }
 *
 * VERIFIER incremente un compteur d'erreurs en cas d'echec,
 * TERMINER_TEST imprime [OK] ou [ECHEC] et retourne 0/1 a l'OS.
 */

#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test_erreurs = 0;

#define VERIFIER(condition)                                                   \
    do {                                                                      \
        if (!(condition)) {                                                   \
            fprintf(stderr, "[ECHEC] %s:%d : %s\n", __FILE__, __LINE__,       \
                    #condition);                                              \
            test_erreurs++;                                                   \
        }                                                                     \
    } while (0)

#define TERMINER_TEST(nom)                                                    \
    do {                                                                      \
        if (test_erreurs == 0) {                                              \
            printf("[OK] %s\n", nom);                                         \
            return 0;                                                         \
        }                                                                     \
        fprintf(stderr, "[ECHEC] %s : %d verification(s)\n", nom,             \
                test_erreurs);                                                \
        return 1;                                                             \
    } while (0)

/* Helper : redirige stdin vers /dev/null pour que les fonctions interactives
 * (chainage_arriere, lire_oui_non) recoivent immediatement EOF au lieu de
 * bloquer sur le terminal. A appeler en debut de main si besoin. */
static inline void test_couper_stdin(void)
{
    if (freopen("/dev/null", "r", stdin) == NULL) {
        fprintf(stderr, "[AVERTISSEMENT] freopen(/dev/null) a echoue.\n");
    }
}

/* Helper : alimente stdin avec une chaine donnee (utile pour tester les
 * fonctions de menu qui attendent une saisie). */
static inline int test_alimenter_stdin(const char *texte)
{
    static char chemin[64] = "/tmp/yamaha_diag_stdin_test.txt";
    FILE *f = fopen(chemin, "w");
    if (f == NULL) return 0;
    fputs(texte, f);
    fclose(f);
    return freopen(chemin, "r", stdin) != NULL;
}

#endif /* TEST_SUPPORT_H */
