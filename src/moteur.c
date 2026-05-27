/* moteur.c - moteur d'inferences par chainage avant et arriere */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moteur.h"
#include "base_faits.h"
#include "solutions.h"

/* dictionnaire pour afficher les questions en langage naturel */
static const BaseSolutions *g_dico = NULL;

/* table des questions deja posees ; les reponses positives deviennent des faits */
#define MAX_POSES 256
static char g_poses[MAX_POSES][MAX_NOM];
static int  g_nb_poses = 0;

/* buts en cours de verification, pour interrompre une dependance cyclique */
#define MAX_PILE 256
static char g_pile[MAX_PILE][MAX_NOM];
static int  g_nb_pile = 0;

void moteur_set_dictionnaire(const struct BaseSolutions *bs)
{
    g_dico = bs;
}

void moteur_reinit(void)
{
    g_nb_poses = 0;
    g_nb_pile = 0;
}

/* retourne 1 si le litteral a deja ete pose */
static int deja_pose(const char *nom)
{
    int i;
    for (i = 0; i < g_nb_poses; i++) {
        if (strcmp(g_poses[i], nom) == 0)
            return 1;
    }
    return 0;
}

static void marquer_pose(const char *nom)
{
    if (g_nb_poses < MAX_POSES) {
        strncpy(g_poses[g_nb_poses], nom, MAX_NOM - 1);
        g_poses[g_nb_poses][MAX_NOM - 1] = '\0';
        g_nb_poses++;
    }
}

static int dans_pile(const char *nom)
{
    int i;
    for (i = 0; i < g_nb_pile; i++) {
        if (strcmp(g_pile[i], nom) == 0)
            return 1;
    }
    return 0;
}

static int empiler_but(const char *nom)
{
    if (g_nb_pile >= MAX_PILE)
        return 0;

    strncpy(g_pile[g_nb_pile], nom, MAX_NOM - 1);
    g_pile[g_nb_pile][MAX_NOM - 1] = '\0';
    g_nb_pile++;
    return 1;
}

static void depiler_but(void)
{
    if (g_nb_pile > 0)
        g_nb_pile--;
}

static int est_conclusion(const char *but, const BaseRegles *br)
{
    const Regle *r = br->regles;

    while (r != NULL) {
        if (strcmp(r->conclusion, but) == 0)
            return 1;
        r = r->suivant;
    }
    return 0;
}

/* ================================================================== */
/*  CHAINAGE AVANT                                                     */
/* ================================================================== */

int chainage_avant(const BaseRegles *br, BaseFaits *bf)
{
    int total = 0;
    int nouveaux;

    /* boucle jusqu'a stabilite */
    do {
        const Regle *r;
        nouveaux = 0;

        r = br->regles;
        while (r != NULL) {
            /* verifier toutes les hypotheses */
            const Litteral *h = r->hypotheses;
            int ok = 1;

            while (h != NULL && ok) {
                if (!fait_existe(bf, h->nom))
                    ok = 0;
                h = h->suivant;
            }

            /* si toutes verifiees et conclusion pas encore connue */
            if (ok && !fait_existe(bf, r->conclusion)) {
                ajouter_fait(bf, r->conclusion);
                printf("  + Deduit : %s\n", r->conclusion);
                nouveaux++;
                total++;
            }

            r = r->suivant;
        }
    } while (nouveaux > 0);

    printf("\n  %d nouveau(x) fait(s) deduit(s).\n", total);
    return total;
}

/* ================================================================== */
/*  CHAINAGE ARRIERE - interne                                         */
/* ================================================================== */

/* prototypes internes */
static int verifier_but(const char *but, const BaseRegles *br,
                        BaseFaits *bf, int interactif, int continu);

static int tenter_regles(const char *but, const BaseRegles *br,
                         BaseFaits *bf, int interactif, int continu,
                         int *a_regle)
{
    const Regle *r = br->regles;

    *a_regle = 0;
    while (r != NULL) {
        if (strcmp(r->conclusion, but) == 0) {
            const Litteral *h = r->hypotheses;
            int ok = 1;

            *a_regle = 1;
            while (h != NULL && ok) {
                if (!verifier_but(h->nom, br, bf, interactif, continu))
                    ok = 0;
                h = h->suivant;
            }

            if (ok) {
                ajouter_fait(bf, but);
                return 1;
            }
        }
        r = r->suivant;
    }
    return 0;
}

static int verifier_but(const char *but, const BaseRegles *br,
                        BaseFaits *bf, int interactif, int continu)
{
    int a_regle;
    int resultat;

    /* deja dans la base */
    if (fait_existe(bf, but))
        return 1;

    /* Un cycle ne prouve rien ; une autre regle du but peut encore aboutir. */
    if (dans_pile(but) || !empiler_but(but))
        return 0;

    resultat = tenter_regles(but, br, bf, interactif, continu, &a_regle);
    depiler_but();
    if (resultat)
        return 1;

    /* Un but defini par des regles doit etre deduit, pas declare par saisie. */
    if (a_regle)
        return 0;

    if (!interactif)
        return 0;

    /* Une reponse negative ne doit pas provoquer la meme question a nouveau. */
    if (deja_pose(but))
        return 0;

    /* poser la question a l'utilisateur */
    {
        char rep[16];
        const char *label = NULL;

        if (g_dico != NULL)
            label = trouver_question(g_dico, but);

        if (label != NULL)
            printf("  > %s (oui/non) : ", label);
        else
            printf("  > %s ? (oui/non) : ", but);

        if (scanf("%15s", rep) != 1) {
            while (getchar() != '\n');
            marquer_pose(but);
            return 0;
        }
        while (getchar() != '\n');

        marquer_pose(but);

        if (rep[0] == 'o' || rep[0] == 'O') {
            ajouter_fait(bf, but);
            return 1;
        }
    }
    return 0;
}

/* ================================================================== */
/*  API publique                                                       */
/* ================================================================== */

int chainage_arriere(const char *but, const BaseRegles *br, BaseFaits *bf)
{
    moteur_reinit();

    if (!fait_existe(bf, but) && !est_conclusion(but, br)) {
        printf("\n  [!] But inconnu : aucune regle ne conclut '%s'.\n", but);
        printf("  Utilisez une conclusion de regle, par exemple "
               "diag_fusible_principal.\n");
        return 0;
    }

    return verifier_but(but, br, bf, 1, 0);
}

int chainage_arriere_continu(const char *but, const BaseRegles *br, BaseFaits *bf)
{
    return verifier_but(but, br, bf, 1, 1);
}
