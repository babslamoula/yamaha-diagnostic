/* solutions.c - chargement et affichage des solutions + dictionnaire questions
 * format solutions : diagnostic|nom|description|solution|cout
 * format questions : litteral|texte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solutions.h"
#include "util.h"

void init_base_solutions(BaseSolutions *bs)
{
    bs->solutions = NULL;
    bs->nb_solutions = 0;
    bs->questions = NULL;
    bs->nb_questions = 0;
}

/* trim local etait duplique avec fichiers.c (audit P2 #13).
 * On delegue a util_trim() pour eviter la divergence (l'ancien trim ici
 * geriait \r alors que celui de fichiers.c ne le faisait pas). */
#define trim(s) util_trim(s)

/* ================================================================== */
/*  CHARGEMENT DES SOLUTIONS                                           */
/* ================================================================== */

int charger_solutions(BaseSolutions *bs, const char *fichier)
{
    FILE *fp;
    char ligne[1024];
    char categorie_courante[128] = "";
    int nb = 0;

    if (bs == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "r");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible d'ouvrir '%s'\n", fichier);
        return 0;
    }

    while (fgets(ligne, sizeof(ligne), fp) != NULL) {
        char *diag, *nom, *desc, *sol, *cout;
        Solution *s;

        /* Lignes vides */
        if (ligne[0] == '\n' || ligne[0] == '\r')
            continue;

        /* Commentaires : detecter les lignes de categorie (# NOM_CATEGORIE) */
        if (ligne[0] == '#') {
            char *cat = ligne + 1;
            size_t l;
            while (*cat == ' ') cat++;
            l = strlen(cat);
            while (l > 0 && (cat[l-1] == '\n' || cat[l-1] == '\r' || cat[l-1] == ' '))
                cat[--l] = '\0';
            if (l > 0) {
                strncpy(categorie_courante, cat, 127);
                categorie_courante[127] = '\0';
            }
            continue;
        }

        diag = strtok(ligne, "|");
        nom  = strtok(NULL, "|");
        desc = strtok(NULL, "|");
        sol  = strtok(NULL, "|");
        cout = strtok(NULL, "|");

        if (diag == NULL || nom == NULL || desc == NULL || sol == NULL || cout == NULL)
            continue;

        s = (Solution *)malloc(sizeof(Solution));
        if (s == NULL) continue;

        strncpy(s->diagnostic, trim(diag), MAX_NOM - 1);
        s->diagnostic[MAX_NOM - 1] = '\0';
        strncpy(s->categorie, categorie_courante, 127);
        s->categorie[127] = '\0';
        strncpy(s->nom, trim(nom), 127);
        s->nom[127] = '\0';
        strncpy(s->description, trim(desc), MAX_DESC - 1);
        s->description[MAX_DESC - 1] = '\0';
        strncpy(s->solution, trim(sol), MAX_SOL - 1);
        s->solution[MAX_SOL - 1] = '\0';
        strncpy(s->cout, trim(cout), MAX_COUT - 1);
        s->cout[MAX_COUT - 1] = '\0';

        s->suivant = bs->solutions;
        bs->solutions = s;
        bs->nb_solutions++;
        nb++;
    }

    fclose(fp);
    printf("  %d solution(s) chargee(s) depuis '%s'\n", nb, fichier);
    return nb;
}

/* ================================================================== */
/*  CHARGEMENT DU DICTIONNAIRE DE QUESTIONS                            */
/* ================================================================== */

int charger_questions(BaseSolutions *bs, const char *fichier)
{
    FILE *fp;
    char ligne[512];
    char categorie_courante[128] = "";
    int nb = 0;

    if (bs == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "r");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible d'ouvrir '%s'\n", fichier);
        return 0;
    }

    while (fgets(ligne, sizeof(ligne), fp) != NULL) {
        char *lit, *desc;
        Question *q;

        if (ligne[0] == '\n' || ligne[0] == '\r')
            continue;

        /* Commentaires : detecter les lignes de categorie (# === CAT ===) */
        if (ligne[0] == '#') {
            char *cat = ligne + 1;
            size_t l;
            while (*cat == ' ') cat++;
            /* Retirer le marqueur "=== " en debut et " ===" en fin */
            if (strncmp(cat, "=== ", 4) == 0) {
                char *end;
                cat += 4;
                end = strstr(cat, " ===");
                if (end) *end = '\0';
            }
            l = strlen(cat);
            while (l > 0 && (cat[l-1] == '\n' || cat[l-1] == '\r' || cat[l-1] == ' '))
                cat[--l] = '\0';
            if (l > 0) {
                strncpy(categorie_courante, cat, 127);
                categorie_courante[127] = '\0';
            }
            continue;
        }

        lit  = strtok(ligne, "|");
        desc = strtok(NULL, "|");

        if (lit == NULL || desc == NULL) continue;

        q = (Question *)malloc(sizeof(Question));
        if (q == NULL) continue;

        strncpy(q->litteral, trim(lit), MAX_NOM - 1);
        q->litteral[MAX_NOM - 1] = '\0';
        strncpy(q->categorie, categorie_courante, 127);
        q->categorie[127] = '\0';
        strncpy(q->description, trim(desc), MAX_DESC - 1);
        q->description[MAX_DESC - 1] = '\0';

        q->suivant = bs->questions;
        bs->questions = q;
        bs->nb_questions++;
        nb++;
    }

    fclose(fp);
    printf("  %d question(s) chargee(s) depuis '%s'\n", nb, fichier);
    return nb;
}

/* ================================================================== */
/*  RECHERCHE                                                          */
/* ================================================================== */

const Solution *trouver_solution(const BaseSolutions *bs, const char *diag)
{
    const Solution *s;
    if (bs == NULL || diag == NULL) return NULL;
    s = bs->solutions;
    while (s != NULL) {
        if (strcmp(s->diagnostic, diag) == 0)
            return s;
        s = s->suivant;
    }
    return NULL;
}

const char *trouver_question(const BaseSolutions *bs, const char *litteral)
{
    const Question *q;
    if (bs == NULL || litteral == NULL) return NULL;
    q = bs->questions;
    while (q != NULL) {
        if (strcmp(q->litteral, litteral) == 0)
            return q->description;
        q = q->suivant;
    }
    return NULL;
}

/* ================================================================== */
/*  AFFICHAGE                                                          */
/* ================================================================== */

void afficher_solution(const Solution *sol)
{
    if (sol == NULL) return;

    printf("\n");
    printf("  +=====================================================+\n");
    printf("  |          DIAGNOSTIC IDENTIFIE                       |\n");
    printf("  +=====================================================+\n");
    printf("  |\n");
    printf("  |  Probleme :\n");
    printf("  |    %s\n", sol->nom);
    printf("  |\n");
    printf("  |  Explication :\n");
    printf("  |    %s\n", sol->description);
    printf("  |\n");
    printf("  |  Solution recommandee :\n");
    printf("  |    %s\n", sol->solution);
    printf("  |\n");
    printf("  |  Cout estime : %s EUR\n", sol->cout);
    printf("  |\n");
    printf("  +=====================================================+\n");
    printf("\n");
}

/* ================================================================== */
/*  LIBERATION MEMOIRE                                                 */
/* ================================================================== */

void liberer_base_solutions(BaseSolutions *bs)
{
    Solution *s, *snext;
    Question *q, *qnext;

    if (bs == NULL) return;

    s = bs->solutions;
    while (s != NULL) {
        snext = s->suivant;
        free(s);
        s = snext;
    }

    q = bs->questions;
    while (q != NULL) {
        qnext = q->suivant;
        free(q);
        q = qnext;
    }

    bs->solutions = NULL;
    bs->nb_solutions = 0;
    bs->questions = NULL;
    bs->nb_questions = 0;
}
