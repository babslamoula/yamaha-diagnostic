/* interface.h - menus et saisies console */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "solutions.h"
#include "base_faits.h"

void effacer_ecran(void);
void pause_ecran(void);
void afficher_banniere(void);

int menu_principal(void);
int menu_gestion_regles(void);
int menu_gestion_faits(void);

/* liste les .txt dans 'dossier' et laisse l'utilisateur choisir */
void selectionner_fichier_dossier(char *buffer, int taille, const char *dossier);
void demander_fichier(char *buffer, int taille, const char *message);

int menu_selection_modele(void);
int menu_selection_annee(int modele);

/* Sous-menu chainage arriere : deux niveaux (categorie puis panne).
 * Remplit 'but' avec l'identifiant diagnostic selectionne.
 * Retourne 1 si une panne a ete choisie, 0 si annule. */
int menu_chainage_arriere(const BaseSolutions *bs, char *but, int taille_but);

/* Saisie interactive des symptomes pour le chainage avant.
 * Pose des questions oui/non par categorie et remplit bf.
 * Retourne le nombre de symptomes ajoutes. */
int menu_saisie_symptomes(const BaseSolutions *bs, BaseFaits *bf);

/* Affiche les diagnostics (diag_*) nouvellement deduits par chainage avant.
 * bf_avant peut etre NULL pour afficher tous les diag_* de bf_apres. */
void afficher_diagnostics_chaines(const BaseFaits *bf_avant,
                                   const BaseFaits *bf_apres,
                                   const BaseSolutions *bs);

#endif /* INTERFACE_H */
