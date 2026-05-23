/* interface.h - menus et saisies console */

#ifndef INTERFACE_H
#define INTERFACE_H

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

#endif /* INTERFACE_H */
