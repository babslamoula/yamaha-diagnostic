/* test_fichiers.c - tests unitaires du chargement / sauvegarde
 *
 * Note importante : depuis le correctif audit P1 #5, charger_base_faits()
 * et charger_base_regles() retournent le NOMBRE d'entrees chargees (et 0
 * en cas d'erreur), plus simplement 1 / 0.
 */

#include <stdio.h>

#include "base_faits.h"
#include "base_regles.h"
#include "fichiers.h"
#include "test_support.h"

#define TEMP_FAITS  "/tmp/yamaha_diag_test_faits.txt"
#define TEMP_REGLES "/tmp/yamaha_diag_test_regles.txt"

int main(void)
{
    BaseFaits faits;
    BaseFaits faits_recharges;
    BaseRegles regles;
    BaseRegles regles_rechargees;
    int nb;

    init_base_faits(&faits);
    init_base_faits(&faits_recharges);
    init_base_regles(&regles);
    init_base_regles(&regles_rechargees);

    /* --- Chargement de data/test_faits.txt --- */
    /* Format attendu : un fait par ligne ("B ;\nC ;\n"). charger_base_faits
     * renvoie le nombre de faits charges. */
    nb = charger_base_faits(&faits, "data/test_faits.txt");
    VERIFIER(nb == 2);
    VERIFIER(faits.nb_faits == 2);
    VERIFIER(fait_existe(&faits, "B") == 1);
    VERIFIER(fait_existe(&faits, "C") == 1);

    /* --- Sauvegarde puis rechargement --- */
    VERIFIER(sauvegarder_base_faits(&faits, TEMP_FAITS) == 1);
    nb = charger_base_faits(&faits_recharges, TEMP_FAITS);
    VERIFIER(nb == 2);
    VERIFIER(faits_recharges.nb_faits == 2);
    VERIFIER(fait_existe(&faits_recharges, "B") == 1);
    VERIFIER(fait_existe(&faits_recharges, "C") == 1);

    /* --- Chargement de data/test_regles.txt (9 lignes valides) --- */
    nb = charger_base_regles(&regles, "data/test_regles.txt");
    VERIFIER(nb == 9);
    VERIFIER(regles.nb_regles == 9);

    /* --- Sauvegarde puis rechargement, le nombre doit etre preserve --- */
    VERIFIER(sauvegarder_base_regles(&regles, TEMP_REGLES) == 1);
    nb = charger_base_regles(&regles_rechargees, TEMP_REGLES);
    VERIFIER(nb == 9);
    VERIFIER(regles_rechargees.nb_regles == 9);

    /* --- Fichier absent : doit retourner 0, ne pas planter --- */
    VERIFIER(charger_base_faits(&faits, "data/fichier_absent.txt") == 0);
    VERIFIER(charger_base_regles(&regles, "data/fichier_absent.txt") == 0);
    /* Les bases existantes ne doivent pas avoir ete modifiees */
    VERIFIER(faits.nb_faits == 2);
    VERIFIER(regles.nb_regles == 9);

    /* Nettoyage */
    remove(TEMP_FAITS);
    remove(TEMP_REGLES);
    liberer_base_faits(&faits);
    liberer_base_faits(&faits_recharges);
    liberer_base_regles(&regles);
    liberer_base_regles(&regles_rechargees);
    TERMINER_TEST("fichiers");
}
