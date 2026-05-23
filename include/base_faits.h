/* base_faits.h - gestion de la base de faits */

#ifndef BASE_FAITS_H
#define BASE_FAITS_H

#include "structures.h"

void init_base_faits(BaseFaits *bf);

/* retourne 1 si ajoute, 0 si deja present ou erreur */
int ajouter_fait(BaseFaits *bf, const char *nom);

/* retourne 1 si supprime, 0 si non trouve */
int supprimer_fait(BaseFaits *bf, const char *nom);

/* retourne 1 si present, 0 sinon */
int fait_existe(const BaseFaits *bf, const char *nom);

void afficher_base_faits(const BaseFaits *bf);
void liberer_base_faits(BaseFaits *bf);
void vider_base_faits(BaseFaits *bf);

/* copie profonde, retourne 1 si ok */
int copier_base_faits(BaseFaits *dest, const BaseFaits *src);

#endif /* BASE_FAITS_H */
