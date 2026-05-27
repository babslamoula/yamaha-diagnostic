/* test_structures.c - tests unitaires des briques de base
 *   - creer_regle / liberer_regle
 *   - ajouter_hypothese (et la limite MAX_HYPOTHESES)
 *   - creer_fait
 *   - liberer_litteraux(NULL) doit etre safe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "test_support.h"

int main(void)
{
    Regle *r = creer_regle();
    Fait *f;
    Litteral *l;
    char nom[16];
    int i;

    /* Construction d'une regle vide */
    VERIFIER(r != NULL);
    VERIFIER(r->hypotheses == NULL);
    VERIFIER(r->nb_hypotheses == 0);
    VERIFIER(r->conclusion[0] == '\0');
    VERIFIER(r->suivant == NULL);

    /* Remplissage jusqu'a la limite MAX_HYPOTHESES (declaree dans structures.h) */
    for (i = 0; i < MAX_HYPOTHESES; i++) {
        snprintf(nom, sizeof(nom), "h%d", i);
        VERIFIER(ajouter_hypothese(r, nom) == 1);
    }
    VERIFIER(r->nb_hypotheses == MAX_HYPOTHESES);

    /* Hypothese supplementaire refusee (et signalee sur stderr par la fonction) */
    VERIFIER(ajouter_hypothese(r, "trop") == 0);
    VERIFIER(r->nb_hypotheses == MAX_HYPOTHESES);

    /* Verification de l'ordre d'insertion (queue de liste) */
    l = r->hypotheses;
    for (i = 0; i < MAX_HYPOTHESES && l != NULL; i++) {
        snprintf(nom, sizeof(nom), "h%d", i);
        VERIFIER(strcmp(l->nom, nom) == 0);
        l = l->suivant;
    }
    VERIFIER(l == NULL);

    /* Robustesse de creer_fait */
    f = creer_fait("symptome");
    VERIFIER(f != NULL);
    VERIFIER(strcmp(f->nom, "symptome") == 0);
    VERIFIER(f->suivant == NULL);

    /* liberer_litteraux(NULL) ne doit pas planter */
    liberer_regle(r);
    liberer_litteraux(NULL);
    free(f);

    TERMINER_TEST("structures");
}
