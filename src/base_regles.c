/* base_regles.c - gestion de la liste chainee de regles */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_regles.h"
#include "solutions.h"
#include "util.h"

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
        vider_stdin();
        return NULL;
    }
    vider_stdin();

    for (i = 0; i < nb; i++) {
        printf("  Hypothese %d : ", i + 1);
        if (scanf("%63s", buffer) != 1) {
            liberer_regle(r);
            return NULL;
        }
        vider_stdin();
        if (!ajouter_hypothese(r, buffer)) {
            liberer_regle(r);
            return NULL;
        }
    }

    printf("  Conclusion : ");
    if (scanf("%63s", buffer) != 1) {
        liberer_regle(r);
        vider_stdin();
        return NULL;
    }
    vider_stdin();

    strncpy(r->conclusion, buffer, MAX_NOM - 1);
    r->conclusion[MAX_NOM - 1] = '\0';

    return r;
}

/* ================================================================== */
/*  VALIDATION DE LA BASE DE REGLES (audit P3 #19)                     */
/* ================================================================== */

/* Verifie : auto-reference (conclusion in hypotheses), cycle direct
 * entre deux regles, litteral utilise sans question correspondante. */
int valider_base_regles(const BaseRegles *br, const struct BaseSolutions *bs)
{
    int avertissements = 0;
    const Regle *r;

    if (br == NULL)
        return 0;

    for (r = br->regles; r != NULL; r = r->suivant) {
        const Litteral *h;
        const Regle *r2;

        /* 1) auto-reference */
        for (h = r->hypotheses; h != NULL; h = h->suivant) {
            if (strcmp(h->nom, r->conclusion) == 0) {
                fprintf(stderr, "  [WARN] auto-reference : '%s' apparait dans ses propres hypotheses.\n",
                        r->conclusion);
                avertissements++;
                break;
            }
        }

        /* 2) cycle direct A->B, B->A : on cherche une regle dont la conclusion
         *    est une hypothese de r ET dont une hypothese est la conclusion de r */
        for (h = r->hypotheses; h != NULL; h = h->suivant) {
            for (r2 = br->regles; r2 != NULL; r2 = r2->suivant) {
                if (r2 == r) continue;
                if (strcmp(r2->conclusion, h->nom) == 0) {
                    const Litteral *h2;
                    for (h2 = r2->hypotheses; h2 != NULL; h2 = h2->suivant) {
                        if (strcmp(h2->nom, r->conclusion) == 0) {
                            fprintf(stderr,
                                    "  [WARN] cycle direct : '%s' <-> '%s'.\n",
                                    r->conclusion, r2->conclusion);
                            avertissements++;
                            goto next_hyp;
                        }
                    }
                }
            }
next_hyp:   ;
        }

        /* 3) litteral non documente (uniquement pour les s_*) */
        if (bs != NULL) {
            for (h = r->hypotheses; h != NULL; h = h->suivant) {
                if (strncmp(h->nom, "s_", 2) != 0)
                    continue;
                if (trouver_question(bs, h->nom) == NULL) {
                    fprintf(stderr,
                            "  [WARN] litteral sans question : '%s' (regle -> %s).\n",
                            h->nom, r->conclusion);
                    avertissements++;
                }
            }
        }
    }

    if (avertissements > 0)
        fprintf(stderr, "  [WARN] %d avertissement(s) sur la base de regles.\n",
                avertissements);
    return avertissements;
}
