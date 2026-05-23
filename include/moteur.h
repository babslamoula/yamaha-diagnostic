/* moteur.h - moteur d'inferences (chainage avant / arriere) */

#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

struct BaseSolutions;

/* deduit tous les faits possibles, retourne le nombre de nouveaux faits */
int chainage_avant(const BaseRegles *br, BaseFaits *bf);

/* verifie un but par chainage arriere, pose des questions si besoin
   retourne 1 si verifie, 0 sinon */
int chainage_arriere(const char *but, const BaseRegles *br, BaseFaits *bf);

/* version akinator : conserve les reponses entre les tentatives */
int chainage_arriere_continu(const char *but, const BaseRegles *br, BaseFaits *bf);

/* reinitialise le suivi des questions posees */
void moteur_reinit(void);

/* definit le dictionnaire pour afficher les questions en clair */
void moteur_set_dictionnaire(const struct BaseSolutions *bs);

#endif /* MOTEUR_H */
