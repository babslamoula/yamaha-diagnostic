/* main.c - point d'entree du systeme expert Yamaha
 * Projet S6 - ITII P23 - Mars 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "base_regles.h"
#include "base_faits.h"
#include "moteur.h"
#include "fichiers.h"
#include "interface.h"
#include "solutions.h"

#define MAX_CHEMIN 256
#define MAX_DIAG   200

/* prototypes internes */
static void gerer_regles(BaseRegles *br);
static void gerer_faits(BaseFaits *bf);
static void mode_diagnostic(const BaseRegles *br, const BaseFaits *bf,
                            const BaseSolutions *bs);

/* ================================================================== */
/*  MAIN                                                               */
/* ================================================================== */

int main(int argc, char *argv[])
{
    BaseRegles br;
    BaseFaits bf;
    BaseFaits bf_travail;
    BaseSolutions bs;
    int choix;
    char chemin[MAX_CHEMIN];
    char base_dir[MAX_CHEMIN] = ".";
    char data_dir[MAX_CHEMIN];
    char temp_path[MAX_CHEMIN];
    char but[MAX_NOM];

    /* trouver le dossier de l'executable pour localiser /data */
    if (argc > 0) {
        char *slash = strrchr(argv[0], '/');
#ifdef _WIN32
        char *bslash = strrchr(argv[0], '\\');
        if (bslash > slash) slash = bslash;
#endif
        if (slash) {
            int len = slash - argv[0];
            if (len < MAX_CHEMIN - 1) {
                strncpy(base_dir, argv[0], len);
                base_dir[len] = '\0';
            }
        }
    }
    snprintf(data_dir, sizeof(data_dir), "%s/data", base_dir);

    init_base_regles(&br);
    init_base_faits(&bf);
    init_base_faits(&bf_travail);
    init_base_solutions(&bs);

    /* chargement auto des bases Yamaha au demarrage */
    snprintf(temp_path, sizeof(temp_path), "%s/yamaha_regles.txt", data_dir);
    charger_base_regles(&br, temp_path);
    snprintf(temp_path, sizeof(temp_path), "%s/yamaha_solutions.txt", data_dir);
    charger_solutions(&bs, temp_path);
    snprintf(temp_path, sizeof(temp_path), "%s/yamaha_questions.txt", data_dir);
    charger_questions(&bs, temp_path);

    effacer_ecran();
    afficher_banniere();
    pause_ecran();

    do {
        effacer_ecran();
        choix = menu_principal();

        switch (choix) {

        case 1: /* diagnostic akinator */
            if (br.nb_regles == 0) {
                printf("\n  [!] Aucune regle chargee.\n");
                pause_ecran();
                break;
            }
            mode_diagnostic(&br, &bf, &bs);
            pause_ecran();
            break;

        case 2: /* chainage avant */
            if (br.nb_regles == 0) {
                printf("\n  [!] Aucune regle chargee.\n");
                pause_ecran();
                break;
            }
            liberer_base_faits(&bf_travail);
            if (!copier_base_faits(&bf_travail, &bf)) {
                printf("  Erreur copie.\n");
                pause_ecran();
                break;
            }
            chainage_avant(&br, &bf_travail);
            printf("\n  Conserver les faits deduits ? (oui/non) : ");
            {
                char rep[16];
                if (scanf("%15s", rep) == 1) {
                    while (getchar() != '\n');
                    if (rep[0] == 'o' || rep[0] == 'O') {
                        liberer_base_faits(&bf);
                        copier_base_faits(&bf, &bf_travail);
                    }
                } else { while (getchar() != '\n'); }
            }
            pause_ecran();
            break;

        case 3: /* chainage arriere */
            if (br.nb_regles == 0) {
                printf("\n  [!] Aucune regle chargee.\n");
                pause_ecran();
                break;
            }
            printf("\n  But a verifier : ");
            if (scanf("%63s", but) != 1) {
                while (getchar() != '\n')
                    ;
                break;
            }
            while (getchar() != '\n');

            liberer_base_faits(&bf_travail);
            copier_base_faits(&bf_travail, &bf);
            moteur_set_dictionnaire(&bs);

            if (chainage_arriere(but, &br, &bf_travail)) {
                const Solution *sol = trouver_solution(&bs, but);
                if (sol) afficher_solution(sol);
                else printf("\n  Resultat : %s est VRAI\n", but);
            } else {
                printf("\n  Resultat : %s ne peut pas etre verifie.\n", but);
            }
            moteur_set_dictionnaire(NULL);
            pause_ecran();
            break;

        case 4: gerer_regles(&br); break;

        case 5:
            selectionner_fichier_dossier(chemin, MAX_CHEMIN, data_dir);
            if (chemin[0]) charger_base_regles(&br, chemin);
            pause_ecran();
            break;

        case 6:
            demander_fichier(chemin, MAX_CHEMIN, "Fichier sauvegarde regles");
            if (chemin[0]) sauvegarder_base_regles(&br, chemin);
            pause_ecran();
            break;

        case 7:
            printf("\n  === BASE DE REGLES ===\n");
            afficher_base_regles(&br);
            pause_ecran();
            break;

        case 8: gerer_faits(&bf); break;

        case 9:
            selectionner_fichier_dossier(chemin, MAX_CHEMIN, data_dir);
            if (chemin[0]) charger_base_faits(&bf, chemin);
            pause_ecran();
            break;

        case 10:
            demander_fichier(chemin, MAX_CHEMIN, "Fichier sauvegarde faits");
            if (chemin[0]) sauvegarder_base_faits(&bf, chemin);
            pause_ecran();
            break;

        case 11:
            printf("\n  === BASE DE FAITS ===\n");
            afficher_base_faits(&bf);
            pause_ecran();
            break;

        case 0:
            printf("\n  Merci d'avoir utilise le diagnostic Yamaha.\n\n");
            break;

        default:
            printf("\n  [!] Choix invalide.\n");
            pause_ecran();
            break;
        }
    } while (choix != 0);

    liberer_base_regles(&br);
    liberer_base_faits(&bf);
    liberer_base_faits(&bf_travail);
    liberer_base_solutions(&bs);

    return 0;
}

/* ================================================================== */
/*  MODE DIAGNOSTIC AKINATOR                                           */
/* ================================================================== */

/* ignore les diagnostics dont la plateforme/moto/gen ne correspond pas
   aux faits deja poses - evite de poser des questions inutiles */
static int diagnostic_compatible(const BaseRegles *br, const char *diag,
                                 const BaseFaits *bf)
{
    const Regle *r = br->regles;
    while (r != NULL) {
        if (strcmp(r->conclusion, diag) == 0) {
            const Litteral *h = r->hypotheses;
            int ok = 1;
            while (h != NULL && ok) {
                if (strncmp(h->nom, "plateforme_", 11) == 0 ||
                    strncmp(h->nom, "moto_", 5) == 0 ||
                    strncmp(h->nom, "gen_", 4) == 0) {
                    if (!fait_existe(bf, h->nom))
                        ok = 0;
                }
                h = h->suivant;
            }
            if (ok) return 1;
        }
        r = r->suivant;
    }
    return 0;
}

static void mode_diagnostic(const BaseRegles *br, const BaseFaits *bf_orig,
                            const BaseSolutions *bs)
{
    BaseFaits bf_diag;
    char diagnostics[MAX_DIAG][MAX_NOM];
    int nb_diag = 0;
    int modele, annee;
    int i;
    int trouve = 0;
    const Regle *r;

    init_base_faits(&bf_diag);

    if (bf_orig->nb_faits > 0)
        copier_base_faits(&bf_diag, bf_orig);

    /* etape 1 : choisir le modele */
    modele = menu_selection_modele();
    if (modele <= 0) { liberer_base_faits(&bf_diag); return; }

    switch (modele) {
    case 1: ajouter_fait(&bf_diag, "moto_r1");
            ajouter_fait(&bf_diag, "plateforme_cp4"); break;
    case 2: ajouter_fait(&bf_diag, "moto_mt10");
            ajouter_fait(&bf_diag, "plateforme_cp4"); break;
    case 3: ajouter_fait(&bf_diag, "moto_mt07");
            ajouter_fait(&bf_diag, "plateforme_cp2"); break;
    case 4: ajouter_fait(&bf_diag, "moto_tracer700");
            ajouter_fait(&bf_diag, "plateforme_cp2"); break;
    case 5: ajouter_fait(&bf_diag, "moto_tenere700");
            ajouter_fait(&bf_diag, "plateforme_cp2"); break;
    case 6: ajouter_fait(&bf_diag, "moto_xsr700");
            ajouter_fait(&bf_diag, "plateforme_cp2"); break;
    case 7: ajouter_fait(&bf_diag, "moto_mt09");
            ajouter_fait(&bf_diag, "plateforme_cp3"); break;
    case 8: ajouter_fait(&bf_diag, "moto_tracer900");
            ajouter_fait(&bf_diag, "plateforme_cp3"); break;
    case 9: ajouter_fait(&bf_diag, "moto_xsr900");
            ajouter_fait(&bf_diag, "plateforme_cp3"); break;
    case 10: ajouter_fait(&bf_diag, "moto_niken");
             ajouter_fait(&bf_diag, "plateforme_cp3"); break;
    default: liberer_base_faits(&bf_diag); return;
    }

    /* etape 2 : choisir l'annee / generation */
    annee = menu_selection_annee(modele);
    if (annee <= 0) { liberer_base_faits(&bf_diag); return; }

    if (fait_existe(&bf_diag, "plateforme_cp4")) {
        switch (annee) {
        case 1: ajouter_fait(&bf_diag, "gen_4xv"); break;
        case 2: ajouter_fait(&bf_diag, "gen_5pw"); break;
        case 3: ajouter_fait(&bf_diag, "gen_5vy"); break;
        case 4: ajouter_fait(&bf_diag, "gen_4c8"); break;
        case 5: ajouter_fait(&bf_diag, "gen_14b"); break;
        case 6: ajouter_fait(&bf_diag, "gen_2ks"); break;
        case 7: ajouter_fait(&bf_diag, "gen_b3l"); break;
        }
    } else if (fait_existe(&bf_diag, "plateforme_cp2")) {
        switch (annee) {
        case 1: ajouter_fait(&bf_diag, "gen_cp2_ph1"); break;
        case 2: ajouter_fait(&bf_diag, "gen_cp2_ph2"); break;
        case 3: ajouter_fait(&bf_diag, "gen_cp2_ph3"); break;
        }
    } else if (fait_existe(&bf_diag, "plateforme_cp3")) {
        switch (annee) {
        case 1: ajouter_fait(&bf_diag, "gen_847"); break;
        case 2: ajouter_fait(&bf_diag, "gen_890"); break;
        }
    }

    /* etape 3 : lister tous les diagnostics possibles */
    r = br->regles;
    while (r != NULL) {
        if (strncmp(r->conclusion, "diag_", 5) == 0) {
            int deja = 0;
            for (i = 0; i < nb_diag; i++) {
                if (strcmp(diagnostics[i], r->conclusion) == 0) {
                    deja = 1; break;
                }
            }
            if (!deja && nb_diag < MAX_DIAG) {
                strncpy(diagnostics[nb_diag], r->conclusion, MAX_NOM - 1);
                diagnostics[nb_diag][MAX_NOM - 1] = '\0';
                nb_diag++;
            }
        }
        r = r->suivant;
    }

    /* etape 4 : poser les questions et trouver le diagnostic */
    printf("\n  ┌─────────────────────────────────────────────────┐\n");
    printf("  │  Le systeme va analyser votre probleme...       │\n");
    printf("  │  Repondez aux questions par 'oui' ou 'non'.     │\n");
    printf("  └─────────────────────────────────────────────────┘\n\n");

    moteur_set_dictionnaire(bs);
    moteur_reinit();

    for (i = 0; i < nb_diag && !trouve; i++) {
        if (!diagnostic_compatible(br, diagnostics[i], &bf_diag))
            continue;

        if (chainage_arriere_continu(diagnostics[i], br, &bf_diag)) {
            trouve = 1;
            {
                const Solution *sol = trouver_solution(bs, diagnostics[i]);
                if (sol) {
                    afficher_solution(sol);
                } else {
                    printf("\n  Diagnostic identifie : %s\n", diagnostics[i]);
                }
            }
        }
    }

    moteur_set_dictionnaire(NULL);

    if (!trouve) {
        printf("\n  ┌─────────────────────────────────────────────────┐\n");
        printf("  │  Aucun diagnostic identifie avec ces symptomes. │\n");
        printf("  │  Consultez un concessionnaire Yamaha agree.     │\n");
        printf("  └─────────────────────────────────────────────────┘\n");
    }

    liberer_base_faits(&bf_diag);
}

/* ================================================================== */
/*  Sous-menus                                                         */
/* ================================================================== */

static void gerer_regles(BaseRegles *br)
{
    int choix;
    int index;
    Regle *r;

    do {
        effacer_ecran();
        choix = menu_gestion_regles();

        switch (choix) {
        case 1:
            printf("\n  --- Ajout d'une regle ---\n");
            r = creer_regle_interactive();
            if (r) { ajouter_regle_base(br, r); printf("  Ajoutee.\n"); }
            pause_ecran();
            break;
        case 2:
            afficher_base_regles(br);
            printf("  N. de la regle a supprimer : ");
            if (scanf("%d", &index) == 1) {
                while (getchar() != '\n');
                supprimer_regle(br, index);
            } else { while (getchar() != '\n'); }
            pause_ecran();
            break;
        case 3:
            afficher_base_regles(br);
            pause_ecran();
            break;
        case 0: break;
        default: printf("  Invalide.\n"); pause_ecran(); break;
        }
    } while (choix != 0);
}

static void gerer_faits(BaseFaits *bf)
{
    int choix;
    char nom[MAX_NOM];

    do {
        effacer_ecran();
        choix = menu_gestion_faits();

        switch (choix) {
        case 1:
            printf("  Fait (symptome) : ");
            if (scanf("%63s", nom) == 1) {
                while (getchar() != '\n');
                ajouter_fait(bf, nom);
            } else { while (getchar() != '\n'); }
            pause_ecran();
            break;
        case 2:
            afficher_base_faits(bf);
            printf("  Fait a supprimer : ");
            if (scanf("%63s", nom) == 1) {
                while (getchar() != '\n');
                supprimer_fait(bf, nom);
            } else { while (getchar() != '\n'); }
            pause_ecran();
            break;
        case 3:
            afficher_base_faits(bf);
            pause_ecran();
            break;
        case 4:
            vider_base_faits(bf);
            printf("  Reinitialise.\n");
            pause_ecran();
            break;
        case 0: break;
        default: printf("  Invalide.\n"); pause_ecran(); break;
        }
    } while (choix != 0);
}
