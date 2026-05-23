/* solutions.c - chargement et affichage des solutions + dictionnaire questions
 * format solutions : diagnostic|nom|description|solution|cout
 * format questions : litteral|texte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solutions.h"

void init_base_solutions(BaseSolutions *bs)
{
    bs->solutions = NULL;
    bs->nb_solutions = 0;
    bs->questions = NULL;
    bs->nb_questions = 0;
}

/* ------------------------------------------------------------------ */
/*  Utilitaire : lire une ligne et decouper par '|'                    */
/* ------------------------------------------------------------------ */
static char *trim(char *str)
{
    char *end;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '\0') return str;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        end--;
    *(end + 1) = '\0';
    return str;
}

/* ================================================================== */
/*  CHARGEMENT DES SOLUTIONS                                           */
/* ================================================================== */

int charger_solutions(BaseSolutions *bs, const char *fichier)
{
    FILE *fp;
    char ligne[1024];
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

        /* Ignorer commentaires et lignes vides */
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r')
            continue;

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
    return 1;
}

/* ================================================================== */
/*  CHARGEMENT DU DICTIONNAIRE DE QUESTIONS                            */
/* ================================================================== */

int charger_questions(BaseSolutions *bs, const char *fichier)
{
    FILE *fp;
    char ligne[512];
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

        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r')
            continue;

        lit  = strtok(ligne, "|");
        desc = strtok(NULL, "|");

        if (lit == NULL || desc == NULL) continue;

        q = (Question *)malloc(sizeof(Question));
        if (q == NULL) continue;

        strncpy(q->litteral, trim(lit), MAX_NOM - 1);
        q->litteral[MAX_NOM - 1] = '\0';
        strncpy(q->description, trim(desc), MAX_DESC - 1);
        q->description[MAX_DESC - 1] = '\0';

        q->suivant = bs->questions;
        bs->questions = q;
        bs->nb_questions++;
        nb++;
    }

    fclose(fp);
    printf("  %d question(s) chargee(s) depuis '%s'\n", nb, fichier);
    return 1;
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
