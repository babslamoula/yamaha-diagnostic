/* base_regles.c - gestion de la liste chainee de regles */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_regles.h"

void init_base_regles(BaseRegles *br)
{
    br->regles = NULL;
    br->nb_regles = 0;
}

int ajouter_regle_base(BaseRegles *br, Regle *r)
{
    Regle *courant;

    if (br == NULL || r == NULL)
        return 0;

    r->suivant = NULL;

    if (br->regles == NULL) {
        br->regles = r;
    } else {
        courant = br->regles;
        while (courant->suivant != NULL)
            courant = courant->suivant;
        courant->suivant = r;
    }
    br->nb_regles++;
    return 1;
}

int supprimer_regle(BaseRegles *br, int index)
{
    Regle *courant;
    Regle *precedent = NULL;
    int i = 1;

    if (br == NULL || index < 1 || index > br->nb_regles)
        return 0;

    courant = br->regles;
    while (courant != NULL && i < index) {
        precedent = courant;
        courant = courant->suivant;
        i++;
    }

    if (courant == NULL)
        return 0;

    if (precedent == NULL) {
        br->regles = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }

    liberer_regle(courant);
    br->nb_regles--;
    return 1;
}

void afficher_base_regles(const BaseRegles *br)
{
    const Regle *r;
    const Litteral *l;
    int i = 1;

    if (br == NULL || br->regles == NULL) {
        printf("  (base de regles vide)\n");
        return;
    }

    printf("\n  %-4s %-60s %s\n", "N.", "REGLE", "CONCLUSION");
    printf("  ---------------------------------------------------------------"
           "----------\n");

    r = br->regles;
    while (r != NULL) {
        printf("  %-4d ", i);

        /* Afficher les hypotheses */
        l = r->hypotheses;
        while (l != NULL) {
            printf("%s", l->nom);
            if (l->suivant != NULL)
                printf(" ET ");
            l = l->suivant;
        }
        printf(" -> %s\n", r->conclusion);

        r = r->suivant;
        i++;
    }
    printf("\n  Total : %d regle(s)\n", br->nb_regles);
}

void liberer_base_regles(BaseRegles *br)
{
    Regle *courant;
    Regle *suivant;

    if (br == NULL)
        return;

    courant = br->regles;
    while (courant != NULL) {
        suivant = courant->suivant;
        liberer_regle(courant);
        courant = suivant;
    }
    br->regles = NULL;
    br->nb_regles = 0;
}

Regle *creer_regle_interactive(void)
{
    Regle *r;
    char buffer[MAX_NOM];
    int nb;
    int i;

    r = creer_regle();
    if (r == NULL)
        return NULL;

    printf("  Nombre d'hypotheses (1-%d) : ", MAX_HYPOTHESES);
    if (scanf("%d", &nb) != 1 || nb < 1 || nb > MAX_HYPOTHESES) {
        printf("  Nombre invalide.\n");
        liberer_regle(r);
        while (getchar() != '\n');
        return NULL;
    }
    while (getchar() != '\n');

    for (i = 0; i < nb; i++) {
        printf("  Hypothese %d : ", i + 1);
        if (scanf("%32s", buffer) != 1) {
            liberer_regle(r);
            return NULL;
        }
        while (getchar() != '\n');
        if (!ajouter_hypothese(r, buffer)) {
            liberer_regle(r);
            return NULL;
        }
    }

    printf("  Conclusion : ");
    if (scanf("%32s", buffer) != 1) {
        liberer_regle(r);
        while (getchar() != '\n');
        return NULL;
    }
    while (getchar() != '\n');

    strncpy(r->conclusion, buffer, MAX_NOM - 1);
    r->conclusion[MAX_NOM - 1] = '\0';

    return r;
}
