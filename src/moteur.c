/* moteur.c - moteur d'inferences par chainage avant et arriere */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moteur.h"
#include "base_faits.h"
#include "solutions.h"
#include "util.h"

/* dictionnaire pour afficher les questions en langage naturel */
static const BaseSolutions *g_dico = NULL;

/* table des questions deja posees / refusees en mode akinator */
#define MAX_POSES 256
static char g_poses[MAX_POSES][MAX_NOM];
static int  g_nb_poses = 0;

/* pile de buts en cours de resolution - protege contre les recursions
 * infinies dues a un cycle dans la base (A -> B ; B -> A ;).
 * Avant cette protection, verifier_but() <-> tenter_regles() pouvaient
 * s'auto-appeler indefiniment et provoquer un stack overflow. */
#define MAX_PILE_BUTS 64
static char g_pile_buts[MAX_PILE_BUTS][MAX_NOM];
static int  g_nb_pile = 0;

static int but_dans_pile(const char *but)
{
    int i;
    for (i = 0; i < g_nb_pile; i++) {
        if (strcmp(g_pile_buts[i], but) == 0)
            return 1;
    }
    return 0;
}

static int pousser_but(const char *but)
{
    if (g_nb_pile >= MAX_PILE_BUTS)
        return 0;
    strncpy(g_pile_buts[g_nb_pile], but, MAX_NOM - 1);
    g_pile_buts[g_nb_pile][MAX_NOM - 1] = '\0';
    g_nb_pile++;
    return 1;
}

static void depiler_but(void)
{
    if (g_nb_pile > 0)
        g_nb_pile--;
}

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
                         BaseFaits *bf, int interactif, int continu)
{
    const Regle *r = br->regles;
    while (r != NULL) {
        if (strcmp(r->conclusion, but) == 0) {
            const Litteral *h = r->hypotheses;
            int ok = 1;

            while (h != NULL && ok) {
                if (!verifier_but(h->nom, br, bf, interactif, continu))
                    ok = 0;
                h = h->suivant;
            }

            if (ok) return 1;
        }
        r = r->suivant;
    }
    return 0;
}

static int verifier_but(const char *but, const BaseRegles *br,
                        BaseFaits *bf, int interactif, int continu)
{
    int res = 0;

    /* deja dans la base */
    if (fait_existe(bf, but))
        return 1;

    /* protection contre les cycles : si on est deja en train de chercher
     * ce but plus haut dans la chaine d'appels, c'est un cycle, on coupe. */
    if (but_dans_pile(but))
        return 0;

    if (!pousser_but(but)) {
        fprintf(stderr, "  [!] Profondeur de chainage arriere depassee sur '%s'\n", but);
        return 0;
    }

    /* essayer par les regles */
    if (tenter_regles(but, br, bf, interactif, continu)) {
        res = 1;
        goto fin;
    }

    if (!interactif) {
        res = 0;
        goto fin;
    }

    /* mode continu : si deja refuse, ne pas reposer */
    if (continu && deja_pose(but)) {
        res = 0;
        goto fin;
    }

    /* poser la question a l'utilisateur. lire_oui_non() gere EOF, reinvite
     * sur reponse non reconnue et accepte oui/o/y/non/n. */
    {
        const char *label = NULL;
        char invite[MAX_NOM + 64];
        int reponse;

        if (g_dico != NULL)
            label = trouver_question(g_dico, but);

        snprintf(invite, sizeof(invite), "  > %s ? (oui/non) : ",
                 label != NULL ? label : but);

        reponse = lire_oui_non(invite);

        if (continu) marquer_pose(but);

        if (reponse == 1) {
            ajouter_fait(bf, but);
            res = 1;
        } else {
            res = 0;
        }
    }

fin:
    depiler_but();
    return res;
}

/* ================================================================== */
/*  API publique                                                       */
/* ================================================================== */

int chainage_arriere(const char *but, const BaseRegles *br, BaseFaits *bf)
{
    return verifier_but(but, br, bf, 1, 0);
}

int chainage_arriere_continu(const char *but, const BaseRegles *br, BaseFaits *bf)
{
    return verifier_but(but, br, bf, 1, 1);
}
