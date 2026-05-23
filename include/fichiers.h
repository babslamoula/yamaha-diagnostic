/* fichiers.h - chargement/sauvegarde des bases (regles et faits)
 * format regles : "hyp1 hyp2 -> conclusion ;"
 * format faits  : "fait ;"
 */

#ifndef FICHIERS_H
#define FICHIERS_H

#include "structures.h"

/* retournent 1 si ok, 0 si erreur */
int charger_base_regles(BaseRegles *br, const char *fichier);
int sauvegarder_base_regles(const BaseRegles *br, const char *fichier);
int charger_base_faits(BaseFaits *bf, const char *fichier);
int sauvegarder_base_faits(const BaseFaits *bf, const char *fichier);

#endif /* FICHIERS_H */
