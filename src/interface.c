/* interface.c - menus et saisies console */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "interface.h"

void effacer_ecran(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_ecran(void)
{
    printf("\n  Appuyez sur Entree pour continuer...");
    while (getchar() != '\n');
}

void afficher_banniere(void)
{
    printf("\n");
    printf("  ===================================================\n");
    printf("  |                                                 |\n");
    printf("  |      Y A M A H A   D I A G N O S T I C         |\n");
    printf("  |                                                 |\n");
    printf("  |      Systeme Expert - Pannes Moto               |\n");
    printf("  |      Exclusif Yamaha                            |\n");
    printf("  |                                                 |\n");
    printf("  ===================================================\n");
    printf("\n");
}

int menu_principal(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         YAMAHA - MENU PRINCIPAL                 │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │                                                 │\n");
    printf("  │   DIAGNOSTIC                                    │\n");
    printf("  │    1. Diagnostic de panne (Akinator)            │\n");
    printf("  │                                                 │\n");
    printf("  │   MOTEUR D'INFERENCES                           │\n");
    printf("  │    2. Chainage avant (deduire les pannes)       │\n");
    printf("  │    3. Chainage arriere (verifier une panne)     │\n");
    printf("  │                                                 │\n");
    printf("  │   BASE DE REGLES                                │\n");
    printf("  │    4. Gerer les regles                          │\n");
    printf("  │    5. Charger base de regles (fichier)          │\n");
    printf("  │    6. Sauvegarder base de regles (fichier)      │\n");
    printf("  │    7. Afficher base de regles                   │\n");
    printf("  │                                                 │\n");
    printf("  │   BASE DE FAITS                                 │\n");
    printf("  │    8. Gerer les faits (symptomes)               │\n");
    printf("  │    9. Charger base de faits (fichier)           │\n");
    printf("  │   10. Sauvegarder base de faits (fichier)       │\n");
    printf("  │   11. Afficher base de faits                    │\n");
    printf("  │                                                 │\n");
    printf("  │    0. Quitter                                   │\n");
    printf("  │                                                 │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');

    return choix;
}

int menu_gestion_regles(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         GESTION DES REGLES                      │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │  1. Ajouter une regle                           │\n");
    printf("  │  2. Supprimer une regle                         │\n");
    printf("  │  3. Afficher les regles                         │\n");
    printf("  │  0. Retour                                      │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return choix;
}

int menu_gestion_faits(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │         GESTION DES FAITS (SYMPTOMES)           │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │  1. Ajouter un fait                             │\n");
    printf("  │  2. Supprimer un fait                           │\n");
    printf("  │  3. Afficher les faits                          │\n");
    printf("  │  4. Reinitialiser les faits                     │\n");
    printf("  │  0. Retour                                      │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return choix;
}

void demander_fichier(char *buffer, int taille, const char *message)
{
    printf("  %s : ", message);
    if (fgets(buffer, taille, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
    }
}

void selectionner_fichier_dossier(char *buffer, int taille, const char *dossier)
{
    DIR *d;
    struct dirent *dir;
    char fichiers[50][256];
    int nb_fichiers = 0;
    int choix = -1;
    int i;

    d = opendir(dossier);
    if (d) {
        printf("\n  Fichiers .txt disponibles dans '%s/' :\n", dossier);
        
        while ((dir = readdir(d)) != NULL) {
            size_t len = strlen(dir->d_name);
            /* Filtrer les fichiers se terminant par .txt */
            if (len > 4 && strcmp(dir->d_name + len - 4, ".txt") == 0) {
                if (nb_fichiers < 50) {
                    strncpy(fichiers[nb_fichiers], dir->d_name, 255);
                    fichiers[nb_fichiers][255] = '\0';
                    nb_fichiers++;
                }
            }
        }
        closedir(d);

        if (nb_fichiers == 0) {
            printf("  [Aucun fichier .txt trouve]\n");
        } else {
            /* Tri bulles simple pour affichage alphabetique */
            for (i = 0; i < nb_fichiers - 1; i++) {
                int j;
                for (j = 0; j < nb_fichiers - i - 1; j++) {
                    if (strcmp(fichiers[j], fichiers[j+1]) > 0) {
                        char temp[256];
                        strcpy(temp, fichiers[j]);
                        strcpy(fichiers[j], fichiers[j+1]);
                        strcpy(fichiers[j+1], temp);
                    }
                }
            }

            for (i = 0; i < nb_fichiers; i++) {
                printf("  %2d. %s\n", i + 1, fichiers[i]);
            }
        }
    } else {
        printf("\n  [Erreur d'ouverture du dossier '%s']\n", dossier);
    }

    printf("   0. Saisir le chemin manuellement / Annuler\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        buffer[0] = '\0';
        return;
    }
    while (getchar() != '\n');

    if (choix > 0 && choix <= nb_fichiers) {
        snprintf(buffer, taille, "%s/%s", dossier, fichiers[choix - 1]);
    } else if (choix == 0) {
        demander_fichier(buffer, taille, "Saisir manuellement le chemin du fichier");
    } else {
        printf("  [!] Choix invalide.\n");
        buffer[0] = '\0';
    }
}


/* ================================================================== */
/*  SELECTION DU MODELE                                                */
/* ================================================================== */

int menu_selection_modele(void)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │    Selectionnez votre modele Yamaha              │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP4                                │\n");
    printf("  │    1. YZF-R1 / R1M                              │\n");
    printf("  │    2. MT-10 / MT-10 SP                          │\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP2                                │\n");
    printf("  │    3. MT-07 / MT-07 SP                          │\n");
    printf("  │    4. Tracer 700 / Tracer 7                     │\n");
    printf("  │    5. Tenere 700 (T7)                           │\n");
    printf("  │    6. XSR 700                                   │\n");
    printf("  │                                                 │\n");
    printf("  │   Plateforme CP3                                │\n");
    printf("  │    7. MT-09 / MT-09 SP                          │\n");
    printf("  │    8. Tracer 900 / Tracer 9                     │\n");
    printf("  │    9. XSR 900                                   │\n");
    printf("  │   10. Niken                                     │\n");
    printf("  │                                                 │\n");
    printf("  │    0. Annuler                                   │\n");
    printf("  │                                                 │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre moto : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    return choix;
}

/* ================================================================== */
/*  SELECTION DE L'ANNEE / GENERATION                                  */
/* ================================================================== */

int menu_selection_annee(int modele)
{
    int choix = -1;

    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │    Selectionnez la generation / annee            │\n");
    printf("  ├─────────────────────────────────────────────────┤\n");

    if (modele <= 2) {
        /* CP4 */
        printf("  │  1. 1998 - 2001 (4XV / 5JJ)                    │\n");
        printf("  │  2. 2002 - 2003 (5PW)                          │\n");
        printf("  │  3. 2004 - 2006 (5VY)                          │\n");
        printf("  │  4. 2007 - 2008 (4C8)                          │\n");
        printf("  │  5. 2009 - 2014 (14B / Crossplane)             │\n");
        printf("  │  6. 2015 - 2019 (2KS / BX4)                   │\n");
        printf("  │  7. 2020 - 2025 (B3L / Euro 5)                │\n");
    } else if (modele <= 6) {
        /* CP2 */
        printf("  │  1. 2014 - 2017 (Phase 1)                      │\n");
        printf("  │  2. 2018 - 2020 (Phase 2)                      │\n");
        printf("  │  3. 2021 - 2025 (Phase 3 / Euro 5)            │\n");
    } else {
        /* CP3 */
        printf("  │  1. 2014 - 2020 (847 cc)                       │\n");
        printf("  │  2. 2021 - 2025 (890 cc / Euro 5)             │\n");
    }

    printf("  │  0. Annuler                                     │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("\n  Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    return choix;
}
