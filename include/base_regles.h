/* base_regles.h - gestion de la base de regles */

#ifndef BASE_REGLES_H
#define BASE_REGLES_H

#include "structures.h"

void init_base_regles(BaseRegles *br);

/* retourne 1 si ok */
int ajouter_regle_base(BaseRegles *br, Regle *r);

/* index 1-based, retourne 1 si ok */
int supprimer_regle(BaseRegles *br, int index);

void afficher_base_regles(const BaseRegles *br);
void liberer_base_regles(BaseRegles *br);

/* cree une regle en posant les questions a l'utilisateur */
Regle *creer_regle_interactive(void);

#endif /* BASE_REGLES_H */
