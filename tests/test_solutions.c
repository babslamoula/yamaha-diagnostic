/* test_solutions.c - tests unitaires de la base de solutions et questions
 *
 * Verifie le chargement des fichiers reels (yamaha_solutions.txt et
 * yamaha_questions.txt) et les fonctions de recherche.
 *
 * Note : depuis le correctif audit P1 #5, charger_solutions() et
 * charger_questions() retournent le NOMBRE d'entrees chargees (et 0
 * en cas d'erreur), plus simplement 1 / 0.
 *
 * Note 2 : l'ancienne API selectionner_symptomes(bs, bf) n'existe plus.
 * La nouvelle saisie de symptomes par menu se fait via
 * menu_saisie_symptomes() dans interface.c (testee dans test_interface.c).
 */

#include <stdio.h>
#include <string.h>

#include "solutions.h"
#include "test_support.h"

int main(void)
{
    BaseSolutions bs;
    const Solution *solution;
    const char *question;

    init_base_solutions(&bs);

    /* --- Chargement des bases reelles du projet --- */
    VERIFIER(charger_solutions(&bs, "data/yamaha_solutions.txt") == 47);
    VERIFIER(charger_questions(&bs, "data/yamaha_questions.txt") == 81);
    VERIFIER(bs.nb_solutions == 47);
    VERIFIER(bs.nb_questions == 81);

    /* --- Recherche d'une solution existante --- */
    solution = trouver_solution(&bs, "diag_fusible_principal");
    VERIFIER(solution != NULL);
    if (solution != NULL) {
        VERIFIER(strcmp(solution->diagnostic, "diag_fusible_principal") == 0);
        VERIFIER(strcmp(solution->categorie, "PANNES DE DEMARRAGE") == 0);
        VERIFIER(strcmp(solution->cout, "25") == 0);
        VERIFIER(solution->nom[0] != '\0');
        VERIFIER(solution->description[0] != '\0');
        VERIFIER(solution->solution[0] != '\0');
    }

    /* --- Recherche d'une solution inexistante --- */
    VERIFIER(trouver_solution(&bs, "diagnostic_inconnu") == NULL);
    VERIFIER(trouver_solution(&bs, "") == NULL);
    VERIFIER(trouver_solution(NULL, "x") == NULL);

    /* --- Recherche d'une question existante --- */
    question = trouver_question(&bs, "s_fusible_principal_hs");
    VERIFIER(question != NULL);
    if (question != NULL) {
        VERIFIER(strstr(question, "fusible principal") != NULL);
    }

    /* --- Recherche d'une question inexistante --- */
    VERIFIER(trouver_question(&bs, "question_inconnue") == NULL);
    VERIFIER(trouver_question(&bs, "") == NULL);
    VERIFIER(trouver_question(NULL, "x") == NULL);

    /* --- Liberation propre --- */
    liberer_base_solutions(&bs);
    VERIFIER(bs.solutions == NULL);
    VERIFIER(bs.questions == NULL);
    VERIFIER(bs.nb_solutions == 0);
    VERIFIER(bs.nb_questions == 0);

    TERMINER_TEST("solutions");
}
