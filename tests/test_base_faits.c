/* test_base_faits.c - tests unitaires de la base de faits
 *   - ajout / suppression / unicite
 *   - copie profonde
 *   - vider_base_faits == liberer_base_faits
 */

#include "base_faits.h"
#include "test_support.h"

int main(void)
{
    BaseFaits source;
    BaseFaits copie;

    init_base_faits(&source);
    init_base_faits(&copie);

    /* Etat initial */
    VERIFIER(source.nb_faits == 0);
    VERIFIER(source.faits == NULL);
    VERIFIER(fait_existe(&source, "absent") == 0);
    VERIFIER(supprimer_fait(&source, "absent") == 0);

    /* Ajouts + anti-doublon */
    VERIFIER(ajouter_fait(&source, "a") == 1);
    VERIFIER(ajouter_fait(&source, "b") == 1);
    VERIFIER(ajouter_fait(&source, "a") == 0);   /* deja present */
    VERIFIER(source.nb_faits == 2);
    VERIFIER(fait_existe(&source, "a") == 1);
    VERIFIER(fait_existe(&source, "b") == 1);

    /* Copie profonde : modifier la source ne doit pas affecter la copie */
    VERIFIER(copier_base_faits(&copie, &source) == 1);
    VERIFIER(copie.nb_faits == 2);
    VERIFIER(fait_existe(&copie, "a") == 1);
    VERIFIER(fait_existe(&copie, "b") == 1);

    VERIFIER(supprimer_fait(&source, "b") == 1);
    VERIFIER(fait_existe(&source, "b") == 0);
    VERIFIER(fait_existe(&copie, "b") == 1);     /* copie intacte */

    /* vider_base_faits doit avoir le meme effet que liberer_base_faits */
    vider_base_faits(&source);
    VERIFIER(source.faits == NULL);
    VERIFIER(source.nb_faits == 0);

    liberer_base_faits(&copie);
    VERIFIER(copie.faits == NULL);
    VERIFIER(copie.nb_faits == 0);

    /* Robustesse : appel sur base vide */
    VERIFIER(supprimer_fait(&source, "n_existe_pas") == 0);

    TERMINER_TEST("base_faits");
}
