/* test_moteur.c - tests unitaires du moteur d'inferences
 *   - chainage_avant : point fixe sur une chaine B -> X -> D -> F
 *   - chainage_arriere : verifie un but, gere les buts inconnus
 *   - chainage_arriere : protection anti-cycle (audit P0 #2)
 *
 * Important : chainage_arriere() est INTERACTIVE - elle interroge stdin
 * via lire_oui_non() lorsqu'une hypothese n'est ni dans la base de faits
 * ni deductible par regle. On coupe donc stdin vers /dev/null pour que
 * lire_oui_non() recoive EOF et que le moteur conclue proprement 0/faux.
 */

#include <string.h>

#include "base_faits.h"
#include "base_regles.h"
#include "moteur.h"
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

static void tester_chainage_avant(void)
{
    BaseRegles br;
    BaseFaits  bf;

    init_base_regles(&br);
    init_base_faits(&bf);

    /* Chaine : B -> X, X -> D, D -> F. En partant de B, on doit deduire X, D, F. */
    ajouter_regle_base(&br, regle_simple("B", "X"));
    ajouter_regle_base(&br, regle_simple("X", "D"));
    ajouter_regle_base(&br, regle_simple("D", "F"));
    ajouter_fait(&bf, "B");

    VERIFIER(chainage_avant(&br, &bf) == 3);
    VERIFIER(fait_existe(&bf, "X") == 1);
    VERIFIER(fait_existe(&bf, "D") == 1);
    VERIFIER(fait_existe(&bf, "F") == 1);
    VERIFIER(bf.nb_faits == 4); /* B + X + D + F */

    /* Un second appel doit etre idempotent (point fixe deja atteint) */
    VERIFIER(chainage_avant(&br, &bf) == 0);

    liberer_base_regles(&br);
    liberer_base_faits(&bf);
}

static void tester_chainage_arriere(void)
{
    BaseRegles br;
    BaseFaits  bf;

    init_base_regles(&br);
    init_base_faits(&bf);

    /* Regle : symptome_present -> diagnostic ; fait : symptome_present */
    ajouter_regle_base(&br, regle_simple("symptome_present", "diagnostic"));
    ajouter_fait(&bf, "symptome_present");

    /* Verification d'un but deductible.
     * chainage_arriere retourne 1 (succes) mais ne memoise PAS le but dans
     * la base de faits : c'est un choix delibere (cf. audit #10). Le main
     * appelant utilise uniquement la valeur de retour pour afficher la
     * solution, il n'inspecte pas bf. */
    VERIFIER(chainage_arriere("diagnostic", &br, &bf) == 1);

    /* But sans regle -> chainage_arriere demande via stdin (=EOF) -> 0 */
    VERIFIER(chainage_arriere("but_inconnu", &br, &bf) == 0);
    VERIFIER(fait_existe(&bf, "but_inconnu") == 0);

    /* But deja present dans la base : retourne 1 immediatement */
    ajouter_fait(&bf, "fait_donne");
    VERIFIER(chainage_arriere("fait_donne", &br, &bf) == 1);

    liberer_base_regles(&br);
    liberer_base_faits(&bf);
}

static void tester_cycle_arriere(void)
{
    BaseRegles br;
    BaseFaits  bf;

    init_base_regles(&br);
    init_base_faits(&bf);

    /* Cycle direct : b -> a et a -> b. Sans protection, la recursion
     * verifier_but() <-> tenter_regles() segfaultait au bout de quelques
     * milliers d'appels. Avec g_pile_buts[64], le but est detecte et la
     * recursion est coupee proprement. */
    ajouter_regle_base(&br, regle_simple("b", "a"));
    ajouter_regle_base(&br, regle_simple("a", "b"));

    /* Doit retourner 0 sans crash et sans rien ajouter a la base de faits */
    VERIFIER(chainage_arriere("a", &br, &bf) == 0);
    VERIFIER(bf.nb_faits == 0);

    liberer_base_regles(&br);
    liberer_base_faits(&bf);
}

int main(void)
{
    /* Coupe stdin vers /dev/null : chainage_arriere() interactive recevra
     * EOF immediatement plutot que bloquer sur le terminal. */
    test_couper_stdin();

    tester_chainage_avant();
    tester_chainage_arriere();
    tester_cycle_arriere();
    TERMINER_TEST("moteur");
}
