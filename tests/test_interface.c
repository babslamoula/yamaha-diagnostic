/* test_interface.c - tests unitaires des menus console
 *
 * Chaque menu lit son choix sur stdin via scanf. On alimente stdin avec
 * test_alimenter_stdin() pour simuler la saisie utilisateur.
 *
 * Note : depuis le correctif audit P1 #5, charger_solutions() retourne
 * le NOMBRE d'entrees chargees (47 pour yamaha_solutions.txt).
 */

#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "solutions.h"
#include "test_support.h"

static void tester_menus_basiques(void)
{
    /* menu_principal */
    VERIFIER(test_alimenter_stdin("3\n") == 1);
    VERIFIER(menu_principal() == 3);

    /* menu_gestion_regles */
    VERIFIER(test_alimenter_stdin("2\n") == 1);
    VERIFIER(menu_gestion_regles() == 2);

    /* menu_gestion_faits */
    VERIFIER(test_alimenter_stdin("4\n") == 1);
    VERIFIER(menu_gestion_faits() == 4);
}

static void tester_selection_modele_annee(void)
{
    /* Modele 7 = MT-09 (plateforme CP3) */
    VERIFIER(test_alimenter_stdin("7\n") == 1);
    VERIFIER(menu_selection_modele() == 7);

    /* Pour la plateforme CP3, generation 2 = 890 cc */
    VERIFIER(test_alimenter_stdin("2\n") == 1);
    VERIFIER(menu_selection_annee(7) == 2);
}

static void tester_demander_fichier(void)
{
    char chemin[128];

    VERIFIER(test_alimenter_stdin("scenario.txt\n") == 1);
    demander_fichier(chemin, sizeof(chemin), "Fichier");
    VERIFIER(strcmp(chemin, "scenario.txt") == 0);
}

static void tester_selecteur_fichier(void)
{
    char chemin[128];

    /* Choix 0 = saisir manuellement le chemin. Le selecteur appelle alors
     * demander_fichier() qui lira "chemin_manuel.txt" sur la ligne suivante. */
    VERIFIER(test_alimenter_stdin("0\nchemin_manuel.txt\n") == 1);
    selectionner_fichier_dossier(chemin, sizeof(chemin), "data");
    VERIFIER(strcmp(chemin, "chemin_manuel.txt") == 0);
}

static void tester_chainage_arriere_menu(void)
{
    BaseSolutions bs;
    char diagnostic[MAX_NOM];

    init_base_solutions(&bs);
    VERIFIER(charger_solutions(&bs, "data/yamaha_solutions.txt") == 47);

    /* Categorie 1 (PANNES DE DEMARRAGE) -> panne 1 (diag_fusible_principal) */
    VERIFIER(test_alimenter_stdin("1\n1\n") == 1);
    VERIFIER(menu_chainage_arriere(&bs, diagnostic, sizeof(diagnostic)) == 1);
    VERIFIER(strcmp(diagnostic, "diag_fusible_principal") == 0);

    /* Annulation au premier niveau (categorie 0) */
    VERIFIER(test_alimenter_stdin("0\n") == 1);
    VERIFIER(menu_chainage_arriere(&bs, diagnostic, sizeof(diagnostic)) == 0);

    liberer_base_solutions(&bs);
}

int main(void)
{
    tester_menus_basiques();
    tester_selection_modele_annee();
    tester_demander_fichier();
    tester_selecteur_fichier();
    tester_chainage_arriere_menu();

    /* Nettoyage du fichier temporaire eventuellement laisse par
     * test_alimenter_stdin(). */
    remove("/tmp/yamaha_diag_stdin_test.txt");

    TERMINER_TEST("interface");
}
