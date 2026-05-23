/* structures.h - types de base du systeme expert */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_NOM        64   /* longueur max d'un identifiant */
#define MAX_HYPOTHESES 10   /* max hypotheses par regle      */

/* element d'une liste chainee d'hypotheses */
typedef struct Litteral {
    char nom[MAX_NOM];
    struct Litteral *suivant;
} Litteral;

/* si hyp1 ET hyp2 ... alors conclusion */
typedef struct Regle {
    Litteral *hypotheses;
    char conclusion[MAX_NOM];
    int nb_hypotheses;
    struct Regle *suivant;
} Regle;

/* un fait connu comme vrai */
typedef struct Fait {
    char nom[MAX_NOM];
    struct Fait *suivant;
} Fait;

typedef struct {
    Regle *regles;
    int nb_regles;
} BaseRegles;

typedef struct {
    Fait *faits;
    int nb_faits;
} BaseFaits;

/* allocation / liberation */
Litteral *creer_litteral(const char *nom);
void      liberer_litteraux(Litteral *tete);
Regle    *creer_regle(void);
int       ajouter_hypothese(Regle *r, const char *nom);
void      liberer_regle(Regle *r);
Fait     *creer_fait(const char *nom);

#endif /* STRUCTURES_H */
