/* interface.c - menus et saisies console */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "interface.h"
#include "solutions.h"
#include "base_faits.h"
#include "util.h"

void effacer_ecran(void)
{
    /* Sequence ANSI : pas de dependance a $PATH, pas de fork.
     * Remplace les anciens system("cls"/"clear") (audit P2 #12). */
    printf("\033[H\033[2J");
    fflush(stdout);
}

void pause_ecran(void)
{
    if (feof(stdin))
        return;
    printf("\n  Appuyez sur Entree pour continuer...");
    vider_stdin();
}

void afficher_banniere(void)
{
    printf("\n");
    printf("  ===================================================\n");
    printf("  |                                                 |\n");
    printf("  |      Y A M A H A   D I A G N O S T I C         |\n");
    printf("  |                                                 |\n");
    printf("  |      Systeme Expert - Pannes Moto               |\n");
    printf("  |      Exclusif Yamaha                            |\n");
    printf("  |                                                 |\n");
    printf("  ===================================================\n");
    printf("\n");
}

int menu_principal(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         YAMAHA - MENU PRINCIPAL                 │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │                                                 │\n");
    printf("  │   DIAGNOSTIC                                    │\n");
    printf("  │    1. Diagnostic de panne (Akinator)            │\n");
    printf("  │                                                 │\n");
    printf("  │   MOTEUR D'INFERENCES                           │\n");
    printf("  │    2. Chainage avant (deduire les pannes)       │\n");
    printf("  │    3. Chainage arriere (verifier une panne)     │\n");
    printf("  │                                                 │\n");
    printf("  │   BASE DE REGLES                                │\n");
    printf("  │    4. Gerer les regles                          │\n");
    printf("  │    5. Charger base de regles (fichier)          │\n");
    printf("  │    6. Sauvegarder base de regles (fichier)      │\n");
    printf("  │    7. Afficher base de regles                   │\n");
    printf("  │                                                 │\n");
    printf("  │   BASE DE FAITS                                 │\n");
    printf("  │    8. Gerer les faits (symptomes)               │\n");
    printf("  │    9. Charger base de faits (fichier)           │\n");
    printf("  │   10. Sauvegarder base de faits (fichier)       │\n");
    printf("  │   11. Afficher base de faits                    │\n");
    printf("  │                                                 │\n");
    printf("  │    0. Quitter                                   │\n");
    printf("  │                                                 │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        return -1;
    }
    vider_stdin();

    return choix;
}

int menu_gestion_regles(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         GESTION DES REGLES                      │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │  1. Ajouter une regle                           │\n");
    printf("  │  2. Supprimer une regle                         │\n");
    printf("  │  3. Afficher les regles                         │\n");
    printf("  │  0. Retour                                      │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        return -1;
    }
    vider_stdin();
    return choix;
}

int menu_gestion_faits(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         GESTION DES FAITS (SYMPTOMES)           │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │  1. Ajouter un fait                             │\n");
    printf("  │  2. Supprimer un fait                           │\n");
    printf("  │  3. Afficher les faits                          │\n");
    printf("  │  4. Reinitialiser les faits                     │\n");
    printf("  │  0. Retour                                      │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        return -1;
    }
    vider_stdin();
    return choix;
}

void demander_fichier(char *buffer, int taille, const char *message)
{
    printf("  %s : ", message);
    if (fgets(buffer, taille, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
    }
}

void selectionner_fichier_dossier(char *buffer, int taille, const char *dossier)
{
    DIR *d;
    struct dirent *dir;
    char fichiers[50][256];
    int nb_fichiers = 0;
    int choix = -1;
    int i;

    d = opendir(dossier);
    if (d) {
        printf("\n  Fichiers .txt disponibles dans '%s/' :\n", dossier);
        
        while ((dir = readdir(d)) != NULL) {
            size_t len = strlen(dir->d_name);
            /* Filtrer les fichiers se terminant par .txt, et ignorer les
             * fichiers caches macOS (.DS_Store, ._*) (audit P2 #11). */
            if (dir->d_name[0] == '.')
                continue;
            if (len > 4 && strcmp(dir->d_name + len - 4, ".txt") == 0) {
                if (nb_fichiers < 50) {
                    strncpy(fichiers[nb_fichiers], dir->d_name, 255);
                    fichiers[nb_fichiers][255] = '\0';
                    nb_fichiers++;
                }
            }
        }
        closedir(d);

        if (nb_fichiers == 0) {
            printf("  [Aucun fichier .txt trouve]\n");
        } else {
            /* Tri bulles simple pour affichage alphabetique */
            for (i = 0; i < nb_fichiers - 1; i++) {
                int j;
                for (j = 0; j < nb_fichiers - i - 1; j++) {
                    if (strcmp(fichiers[j], fichiers[j+1]) > 0) {
                        char temp[256];
                        /* strncpy + terminaison explicite -> jamais d'overflow
                         * meme si readdir donne un nom long (audit P2 #11). */
                        memcpy(temp, fichiers[j], sizeof(temp));
                        memcpy(fichiers[j], fichiers[j+1], sizeof(temp));
                        memcpy(fichiers[j+1], temp, sizeof(temp));
                    }
                }
            }

            for (i = 0; i < nb_fichiers; i++) {
                printf("  %2d. %s\n", i + 1, fichiers[i]);
            }
        }
    } else {
        printf("\n  [Erreur d'ouverture du dossier '%s']\n", dossier);
    }

    printf("   0. Saisir le chemin manuellement / Annuler\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        buffer[0] = '\0';
        return;
    }
    vider_stdin();

    if (choix > 0 && choix <= nb_fichiers) {
        snprintf(buffer, taille, "%s/%s", dossier, fichiers[choix - 1]);
    } else if (choix == 0) {
        demander_fichier(buffer, taille, "Saisir manuellement le chemin du fichier");
    } else {
        printf("  [!] Choix invalide.\n");
        buffer[0] = '\0';
    }
}


/* ================================================================== */
/*  SELECTION DU MODELE                                                */
/* ================================================================== */

int menu_selection_modele(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │    Selectionnez votre modele Yamaha              │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP4                                │\n");
    printf("  │    1. YZF-R1 / R1M                              │\n");
    printf("  │    2. MT-10 / MT-10 SP                          │\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP2                                │\n");
    printf("  │    3. MT-07 / MT-07 SP                          │\n");
    printf("  │    4. Tracer 700 / Tracer 7                     │\n");
    printf("  │    5. Tenere 700 (T7)                           │\n");
    printf("  │    6. XSR 700                                   │\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP3                                │\n");
    printf("  │    7. MT-09 / MT-09 SP                          │\n");
    printf("  │    8. Tracer 900 / Tracer 9                     │\n");
    printf("  │    9. XSR 900                                   │\n");
    printf("  │   10. Niken                                     │\n");
    printf("  │                                                 │\n");
    printf("  │    0. Annuler                                   │\n");
    printf("  │                                                 │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre moto : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        return 0;
    }
    vider_stdin();

    return choix;
}

/* ================================================================== */
/*  SELECTION DE L'ANNEE / GENERATION                                  */
/* ================================================================== */

int menu_selection_annee(int modele)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │    Selectionnez la generation / annee            │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");

    if (modele <= 2) {
        /* CP4 */
        printf("  │  1. 1998 - 2001 (4XV / 5JJ)                    │\n");
        printf("  │  2. 2002 - 2003 (5PW)                          │\n");
        printf("  │  3. 2004 - 2006 (5VY)                          │\n");
        printf("  │  4. 2007 - 2008 (4C8)                          │\n");
        printf("  │  5. 2009 - 2014 (14B / Crossplane)             │\n");
        printf("  │  6. 2015 - 2019 (2KS / BX4)                   │\n");
        printf("  │  7. 2020 - 2025 (B3L / Euro 5)                │\n");
    } else if (modele <= 6) {
        /* CP2 */
        printf("  │  1. 2014 - 2017 (Phase 1)                      │\n");
        printf("  │  2. 2018 - 2020 (Phase 2)                      │\n");
        printf("  │  3. 2021 - 2025 (Phase 3 / Euro 5)            │\n");
    } else {
        /* CP3 */
        printf("  │  1. 2014 - 2020 (847 cc)                       │\n");
        printf("  │  2. 2021 - 2025 (890 cc / Euro 5)             │\n");
    }

    printf("  │  0. Annuler                                     │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_stdin();
        return 0;
    }
    vider_stdin();

    return choix;
}

/* ================================================================== */
/*  MENU CHAINAGE ARRIERE                                              */
/* ================================================================== */

/* Deux niveaux :
 *   1) Choisir une categorie de panne
 *   2) Choisir la panne dans la categorie
 * Remplit 'but' avec le diagnostic selectionne.
 * Retourne 1 si selection valide, 0 si annule. */

#define MENU_MAX_CAT    12
#define MENU_MAX_PANNE  60

int menu_chainage_arriere(const BaseSolutions *bs, char *but, int taille_but)
{
    const Solution *tableau[MENU_MAX_PANNE];
    int nb_total = 0;
    const Solution *s;
    char categories[MENU_MAX_CAT][128];
    int nb_cat = 0;
    int i, j, choix;

    if (bs == NULL || but == NULL) return 0;
    but[0] = '\0';

    /* Collecter toutes les solutions dans un tableau.
     * La liste est en LIFO (prepend), on l'inverse pour retrouver l'ordre fichier. */
    s = bs->solutions;
    while (s != NULL && nb_total < MENU_MAX_PANNE) {
        tableau[nb_total++] = s;
        s = s->suivant;
    }
    for (i = 0; i < nb_total / 2; i++) {
        const Solution *tmp = tableau[i];
        tableau[i] = tableau[nb_total - 1 - i];
        tableau[nb_total - 1 - i] = tmp;
    }

    /* Collecter les categories uniques dans l'ordre d'apparition */
    for (i = 0; i < nb_total; i++) {
        int deja = 0;
        if (tableau[i]->categorie[0] == '\0') continue;
        for (j = 0; j < nb_cat; j++) {
            if (strcmp(categories[j], tableau[i]->categorie) == 0) {
                deja = 1; break;
            }
        }
        if (!deja && nb_cat < MENU_MAX_CAT) {
            strncpy(categories[nb_cat], tableau[i]->categorie, 127);
            categories[nb_cat][127] = '\0';
            nb_cat++;
        }
    }

    if (nb_cat == 0) {
        printf("\n  [!] Aucune panne disponible dans la base de solutions.\n");
        return 0;
    }

    /* -------- Menu niveau 1 : categories -------- */
    /* Boite interieure : 49 caracteres (identique aux autres menus) */
    do {
        effacer_ecran();
        printf("\n");
        printf("  ┌─────────────────────────────────────────────────┐\n");
        printf("  │    CHAINAGE ARRIERE - Choisir une categorie     │\n");
        printf("  ├─────────────────────────────────────────────────┤\n");
        printf("  │                                                 │\n");
        for (i = 0; i < nb_cat; i++) {
            /* %-43.43s : 2 + 2 + 2 + 43 = 49 chars interieurs */
            printf("  │  %2d. %-43.43s│\n", i + 1, categories[i]);
        }
        printf("  │                                                 │\n");
        printf("  │   0. Annuler                                    │\n");
        printf("  └─────────────────────────────────────────────────┘\n");
        printf("\n  Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            vider_stdin();
            choix = -1;
        } else {
            vider_stdin();
        }
        if (choix < 0 || choix > nb_cat)
            printf("\n  [!] Choix invalide.\n");

    } while (choix < 0 || choix > nb_cat);

    if (choix == 0) return 0;

    /* -------- Menu niveau 2 : pannes de la categorie -------- */
    /* Boite interieure : 63 caracteres (boite plus large pour les noms) */
    {
        const char *cat_choisie = categories[choix - 1];
        const Solution *pannes[MENU_MAX_PANNE];
        int nb_pannes = 0;

        for (i = 0; i < nb_total; i++) {
            if (strcmp(tableau[i]->categorie, cat_choisie) == 0 &&
                nb_pannes < MENU_MAX_PANNE) {
                pannes[nb_pannes++] = tableau[i];
            }
        }

        do {
            effacer_ecran();
            printf("\n");
            printf("  ┌───────────────────────────────────────────────────────────────┐\n");
            /* Titre = categorie choisie, 63 chars interieurs : 2 + 61 */
            printf("  │  %-61.61s│\n", cat_choisie);
            printf("  ├───────────────────────────────────────────────────────────────┤\n");
            printf("  │                                                               │\n");
            for (i = 0; i < nb_pannes; i++) {
                /* %-57.57s : 2 + 2 + 2 + 57 = 63 chars interieurs */
                printf("  │  %2d. %-57.57s│\n", i + 1, pannes[i]->nom);
            }
            printf("  │                                                               │\n");
            printf("  │   0. Retour                                                   │\n");
            printf("  └───────────────────────────────────────────────────────────────┘\n");
            printf("\n  Votre choix : ");

            if (scanf("%d", &choix) != 1) {
                vider_stdin();
                choix = -1;
            } else {
                vider_stdin();
            }
            if (choix < 0 || choix > nb_pannes)
                printf("\n  [!] Choix invalide.\n");

        } while (choix < 0 || choix > nb_pannes);

        if (choix == 0) return 0;

        strncpy(but, pannes[choix - 1]->diagnostic, taille_but - 1);
        but[taille_but - 1] = '\0';
        return 1;
    }
}

/* ================================================================== */
/*  SAISIE DES SYMPTOMES (pour chainage avant)                        */
/* ================================================================== */

/* Affiche les questions d'une categorie une par une (oui/non).
 * Ajoute les litteraux confirmes dans bf. Retourne nb ajoutes. */
static int poser_questions_categorie(const Question * const *tableau,
                                     int nb_total,
                                     const char *categorie,
                                     BaseFaits *bf)
{
    char invite[512];
    int i;
    int nb_ajoutes = 0;
    int nb_questions = 0;

    /* Compter les questions de la categorie */
    for (i = 0; i < nb_total; i++) {
        if (strcmp(tableau[i]->categorie, categorie) == 0)
            nb_questions++;
    }

    effacer_ecran();
    printf("\n");
    printf("  ===  %s  ===\n", categorie);
    printf("  (%d question(s)) - Repondez par 'oui' ou 'non'\n\n", nb_questions);

    for (i = 0; i < nb_total; i++) {
        if (strcmp(tableau[i]->categorie, categorie) == 0) {
            int rep;
            snprintf(invite, sizeof(invite),
                     "  > %s\n    (oui/non) : ", tableau[i]->description);
            rep = lire_oui_non(invite);
            if (rep == 1) {
                if (ajouter_fait(bf, tableau[i]->litteral))
                    nb_ajoutes++;
            }
            printf("\n");
        }
    }

    printf("  --- %d symptome(s) saisi(s) pour cette categorie. ---\n", nb_ajoutes);
    pause_ecran();
    return nb_ajoutes;
}

#define SYM_MAX_CAT   8
#define SYM_MAX_Q   120

int menu_saisie_symptomes(const BaseSolutions *bs, BaseFaits *bf)
{
    const Question *tableau[SYM_MAX_Q];
    int nb_total = 0;
    const Question *q;
    char categories[SYM_MAX_CAT][128];
    int nb_cat = 0;
    int i, j, choix;
    int nb_ajoutes_total = 0;

    if (bs == NULL || bf == NULL) return 0;

    /* Collecter les questions en ordre fichier (liste LIFO -> inverser) */
    q = bs->questions;
    while (q != NULL && nb_total < SYM_MAX_Q) {
        tableau[nb_total++] = q;
        q = q->suivant;
    }
    for (i = 0; i < nb_total / 2; i++) {
        const Question *tmp = tableau[i];
        tableau[i] = tableau[nb_total - 1 - i];
        tableau[nb_total - 1 - i] = tmp;
    }

    /* Collecter les categories uniques dans l'ordre d'apparition */
    for (i = 0; i < nb_total; i++) {
        int deja = 0;
        if (tableau[i]->categorie[0] == '\0') continue;
        for (j = 0; j < nb_cat; j++) {
            if (strcmp(categories[j], tableau[i]->categorie) == 0) {
                deja = 1; break;
            }
        }
        if (!deja && nb_cat < SYM_MAX_CAT) {
            strncpy(categories[nb_cat], tableau[i]->categorie, 127);
            categories[nb_cat][127] = '\0';
            nb_cat++;
        }
    }

    if (nb_cat == 0) {
        printf("\n  [!] Aucune question disponible.\n");
        return 0;
    }

    /* Boucle : menu categories -> questions -> retour au menu */
    do {
        effacer_ecran();
        printf("\n");
        printf("  ┌─────────────────────────────────────────────────┐\n");
        printf("  │    CHAINAGE AVANT - Saisie des symptomes        │\n");
        printf("  ├─────────────────────────────────────────────────┤\n");
        printf("  │  Choisissez une categorie et repondez           │\n");
        printf("  │  aux questions. Revenez ici pour en choisir     │\n");
        printf("  │  une autre, ou tapez 0 pour lancer.             │\n");
        printf("  │                                                 │\n");
        for (i = 0; i < nb_cat; i++) {
            printf("  │  %2d. %-43.43s│\n", i + 1, categories[i]);
        }
        printf("  │                                                 │\n");
        printf("  │   0. Lancer le diagnostic                       │\n");
        printf("  └─────────────────────────────────────────────────┘\n");
        printf("\n  Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            vider_stdin();
            choix = -1;
        } else {
            vider_stdin();
        }

        if (choix > 0 && choix <= nb_cat) {
            nb_ajoutes_total += poser_questions_categorie(
                tableau, nb_total, categories[choix - 1], bf);
        } else if (choix != 0) {
            printf("\n  [!] Choix invalide.\n");
        }

    } while (choix != 0);

    return nb_ajoutes_total;
}

/* ================================================================== */
/*  AFFICHAGE DES DIAGNOSTICS DEDUITS (chainage avant)                */
/* ================================================================== */

void afficher_diagnostics_chaines(const BaseFaits *bf_avant,
                                   const BaseFaits *bf_apres,
                                   const BaseSolutions *bs)
{
    const Fait *f;
    int nb_diag = 0;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │    RESULTATS DU CHAINAGE AVANT                  │\n");
    printf("  └─────────────────────────────────────────────────┘\n\n");

    if (bf_apres == NULL) {
        printf("  Aucun resultat.\n");
        return;
    }

    f = bf_apres->faits;
    while (f != NULL) {
        /* N'afficher que les conclusions diag_* nouvellement deduites */
        if (strncmp(f->nom, "diag_", 5) == 0) {
            int nouveau = (bf_avant == NULL) || !fait_existe(bf_avant, f->nom);
            if (nouveau) {
                const Solution *sol = trouver_solution(bs, f->nom);
                nb_diag++;
                printf("  [%d] ", nb_diag);
                if (sol) {
                    printf("%s\n", sol->nom);
                    printf("      Categorie : %s\n", sol->categorie);
                    printf("      Cout estime : %s EUR\n", sol->cout);
                } else {
                    printf("%s\n", f->nom);
                }
                printf("\n");
            }
        }
        f = f->suivant;
    }

    printf("  ─────────────────────────────────────────────────\n");
    if (nb_diag == 0) {
        printf("  Aucune panne identifiee avec ces symptomes.\n");
        printf("  Essayez d'ajouter d'autres symptomes ou\n");
        printf("  consultez le diagnostic Akinator (option 1).\n");
    } else {
        printf("  %d panne(s) identifiee(s) par chainage avant.\n", nb_diag);
    }
    printf("\n");
}
