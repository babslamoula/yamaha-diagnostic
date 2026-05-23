/* solutions.h - solutions de diagnostic + dictionnaire de questions */

#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include "structures.h"

#define MAX_DESC 256
#define MAX_SOL  512
#define MAX_COUT 64

typedef struct Solution {
    char diagnostic[MAX_NOM];
    char nom[128];
    char description[MAX_DESC];
    char solution[MAX_SOL];
    char cout[MAX_COUT];
    struct Solution *suivant;
} Solution;

typedef struct Question {
    char litteral[MAX_NOM];
    char description[MAX_DESC];
    struct Question *suivant;
} Question;

typedef struct BaseSolutions {
    Solution *solutions;
    int nb_solutions;
    Question *questions;
    int nb_questions;
} BaseSolutions;

void            init_base_solutions(BaseSolutions *bs);
int             charger_solutions(BaseSolutions *bs, const char *fichier);
int             charger_questions(BaseSolutions *bs, const char *fichier);
const Solution *trouver_solution(const BaseSolutions *bs, const char *diag);
const char     *trouver_question(const BaseSolutions *bs, const char *litteral);
void            afficher_solution(const Solution *sol);
void            liberer_base_solutions(BaseSolutions *bs);

#endif /* SOLUTIONS_H */
