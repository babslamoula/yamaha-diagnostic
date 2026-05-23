/* base_faits.c - gestion de la liste chainee de faits */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_faits.h"

void init_base_faits(BaseFaits *bf)
{
    bf->faits = NULL;
    bf->nb_faits = 0;
}

int ajouter_fait(BaseFaits *bf, const char *nom)
{
    Fait *f;
    Fait *courant;

    if (bf == NULL || nom == NULL)
        return 0;

    /* pas de doublon */
    courant = bf->faits;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0)
            return 0;
        courant = courant->suivant;
    }

    f = creer_fait(nom);
    if (f == NULL)
        return 0;

    f->suivant = bf->faits;
    bf->faits = f;
    bf->nb_faits++;
    return 1;
}

int supprimer_fait(BaseFaits *bf, const char *nom)
{
    Fait *courant;
    Fait *precedent = NULL;

    if (bf == NULL || nom == NULL)
        return 0;

    courant = bf->faits;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            if (precedent == NULL)
                bf->faits = courant->suivant;
            else
                precedent->suivant = courant->suivant;
            free(courant);
            bf->nb_faits--;
            return 1;
        }
        precedent = courant;
        courant = courant->suivant;
    }
    return 0;
}

int fait_existe(const BaseFaits *bf, const char *nom)
{
    const Fait *f;

    if (bf == NULL || nom == NULL)
        return 0;

    f = bf->faits;
    while (f != NULL) {
        if (strcmp(f->nom, nom) == 0)
            return 1;
        f = f->suivant;
    }
    return 0;
}

void afficher_base_faits(const BaseFaits *bf)
{
    const Fait *f;
    int i = 1;

    if (bf == NULL || bf->faits == NULL) {
        printf("  (base de faits vide)\n");
        return;
    }

    printf("\n");
    f = bf->faits;
    while (f != NULL) {
        printf("  %3d. %s\n", i++, f->nom);
        f = f->suivant;
    }
    printf("\n  Total : %d fait(s)\n", bf->nb_faits);
}

void liberer_base_faits(BaseFaits *bf)
{
    Fait *courant;
    Fait *suivant;

    if (bf == NULL)
        return;

    courant = bf->faits;
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    bf->faits = NULL;
    bf->nb_faits = 0;
}

int copier_base_faits(BaseFaits *dest, const BaseFaits *src)
{
    const Fait *f;

    if (dest == NULL || src == NULL)
        return 0;

    f = src->faits;
    while (f != NULL) {
        if (!ajouter_fait(dest, f->nom))
            return 0;
        f = f->suivant;
    }
    return 1;
}

void vider_base_faits(BaseFaits *bf)
{
    liberer_base_faits(bf);
}
