/* test_base_regles.c - tests unitaires de la base de regles
 *   - tail insertion (l'ordre fichier est preserve)
 *   - suppression par index 1-based
 *   - validation (auto-reference, cycle direct)
 */

#include <string.h>

#include "base_regles.h"
#include "test_support.h"

static Regle *regle_simple(const char *hypothese, const char *conclusion)
{
    Regle *r = creer_regle();

    if (r == NULL || !ajouter_hypothese(r, hypothese)) {
        liberer_regle(r);
        return NULL;
    }
    strncpy(r->conclusion, conclusion, MAX_NOM - 1);
    r->conclusion[MAX_NOM - 1] = '\0';
    return r;
}

static void tester_insertion_suppression(void)
{
    BaseRegles br;

    init_base_regles(&br);

    /* Etat initial */
    VERIFIER(br.regles == NULL);
    VERIFIER(br.nb_regles == 0);
    VERIFIER(supprimer_regle(&br, 1) == 0);

    /* Insertion : tail insert -> l'ordre est preserve */
    VERIFIER(ajouter_regle_base(&br, regle_simple("a", "b")) == 1);
    VERIFIER(ajouter_regle_base(&br, regle_simple("b", "c")) == 1);
    VERIFIER(ajouter_regle_base(&br, regle_simple("c", "d")) == 1);
    VERIFIER(br.nb_regles == 3);
    VERIFIER(strcmp(br.regles->conclusion, "b") == 0);
    VERIFIER(strcmp(br.regles->suivant->conclusion, "c") == 0);
    VERIFIER(strcmp(br.regles->suivant->suivant->conclusion, "d") == 0);

    /* Suppression du milieu */
    VERIFIER(supprimer_regle(&br, 2) == 1);
    VERIFIER(br.nb_regles == 2);
    VERIFIER(strcmp(br.regles->suivant->conclusion, "d") == 0);

    /* Indexes invalides */
    VERIFIER(supprimer_regle(&br, 0) == 0);
    VERIFIER(supprimer_regle(&br, 3) == 0);

    /* Suppression sequentielle jusqu'a base vide */
    VERIFIER(supprimer_regle(&br, 1) == 1);
    VERIFIER(supprimer_regle(&br, 1) == 1);
    VERIFIER(br.regles == NULL);
    VERIFIER(br.nb_regles == 0);

    liberer_base_regles(&br);
}

static void tester_validation(void)
{
    BaseRegles br;
    Regle *auto_ref;

    init_base_regles(&br);

    /* Base saine -> 0 avertissement */
    ajouter_regle_base(&br, regle_simple("a", "b"));
    ajouter_regle_base(&br, regle_simple("b", "c"));
    VERIFIER(valider_base_regles(&br, NULL) == 0);

    /* Auto-reference : a -> a */
    auto_ref = creer_regle();
    if (auto_ref != NULL) {
        ajouter_hypothese(auto_ref, "x");
        strncpy(auto_ref->conclusion, "x", MAX_NOM - 1);
        ajouter_regle_base(&br, auto_ref);
    }
    VERIFIER(valider_base_regles(&br, NULL) >= 1);

    /* Cycle direct : on ajoute "c -> b" (deja existe "b -> c") */
    ajouter_regle_base(&br, regle_simple("c", "b"));
    VERIFIER(valider_base_regles(&br, NULL) >= 2);

    liberer_base_regles(&br);
}

int main(void)
{
    tester_insertion_suppression();
    tester_validation();
    TERMINER_TEST("base_regles");
}
