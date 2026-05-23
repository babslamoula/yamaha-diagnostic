/* structures.c - allocation/liberation des briques de base */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

/* ------------------------------------------------------------------ */
/*  Litteraux                                                          */
/* ------------------------------------------------------------------ */

Litteral *creer_litteral(const char *nom)
{
    Litteral *l = (Litteral *)malloc(sizeof(Litteral));
    if (l == NULL) {
        fprintf(stderr, "Erreur : allocation memoire litteral.\n");
        return NULL;
    }
    strncpy(l->nom, nom, MAX_NOM - 1);
    l->nom[MAX_NOM - 1] = '\0';
    l->suivant = NULL;
    return l;
}

void liberer_litteraux(Litteral *tete)
{
    Litteral *courant = tete;
    Litteral *suivant;
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
}

/* ------------------------------------------------------------------ */
/*  Regles                                                             */
/* ------------------------------------------------------------------ */

Regle *creer_regle(void)
{
    Regle *r = (Regle *)malloc(sizeof(Regle));
    if (r == NULL) {
        fprintf(stderr, "Erreur : allocation memoire regle.\n");
        return NULL;
    }
    r->hypotheses = NULL;
    r->conclusion[0] = '\0';
    r->nb_hypotheses = 0;
    r->suivant = NULL;
    return r;
}

int ajouter_hypothese(Regle *r, const char *nom)
{
    Litteral *nouveau;
    Litteral *courant;

    if (r == NULL || nom == NULL)
        return 0;
    if (r->nb_hypotheses >= MAX_HYPOTHESES) {
        fprintf(stderr, "Erreur : nombre max d'hypotheses atteint (%d).\n",
                MAX_HYPOTHESES);
        return 0;
    }

    nouveau = creer_litteral(nom);
    if (nouveau == NULL)
        return 0;

    /* ajout en fin pour conserver l'ordre de saisie */
    if (r->hypotheses == NULL) {
        r->hypotheses = nouveau;
    } else {
        courant = r->hypotheses;
        while (courant->suivant != NULL)
            courant = courant->suivant;
        courant->suivant = nouveau;
    }
    r->nb_hypotheses++;
    return 1;
}

void liberer_regle(Regle *r)
{
    if (r == NULL)
        return;
    liberer_litteraux(r->hypotheses);
    free(r);
}

/* ------------------------------------------------------------------ */
/*  Faits                                                              */
/* ------------------------------------------------------------------ */

Fait *creer_fait(const char *nom)
{
    Fait *f = (Fait *)malloc(sizeof(Fait));
    if (f == NULL) {
        fprintf(stderr, "Erreur : allocation memoire fait.\n");
        return NULL;
    }
    strncpy(f->nom, nom, MAX_NOM - 1);
    f->nom[MAX_NOM - 1] = '\0';
    f->suivant = NULL;
    return f;
}
