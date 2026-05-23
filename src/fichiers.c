/* fichiers.c - lecture/ecriture des bases de regles et de faits
 * format regles : "hyp1 hyp2 -> conclusion ;"
 * format faits  : "fait ;"
 * les lignes commencant par '#' sont des commentaires
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fichiers.h"
#include "base_regles.h"
#include "base_faits.h"

/* supprime les espaces en debut et fin de chaine */
static char *trim_str(char *s)
{
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return s;
}

/* ================================================================== */
/*  REGLES                                                             */
/* ================================================================== */

int charger_base_regles(BaseRegles *br, const char *fichier)
{
    FILE *fp;
    char ligne[512];
    int nb = 0;

    if (br == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "r");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible d'ouvrir '%s'\n", fichier);
        return 0;
    }

    while (fgets(ligne, sizeof(ligne), fp) != NULL) {
        char *p;
        char *fleche;
        char *semi;
        char *conc;
        Regle *r;
        char token[MAX_NOM];

        /* commentaires et lignes vides */
        p = trim_str(ligne);
        if (p[0] == '#' || p[0] == '\0')
            continue;

        /* trouver la fleche "->" */
        fleche = strstr(p, "->");
        if (fleche == NULL) continue;

        /* trouver le ';' de fin */
        semi = strchr(fleche, ';');

        r = creer_regle();
        if (r == NULL) continue;

        /* hypotheses : tout ce qui est avant "->" */
        *fleche = '\0';
        p = trim_str(p);

        /* decouper par espaces */
        {
            char *tok = strtok(p, " \t");
            while (tok != NULL) {
                strncpy(token, tok, MAX_NOM - 1);
                token[MAX_NOM - 1] = '\0';
                if (strlen(token) > 0)
                    ajouter_hypothese(r, token);
                tok = strtok(NULL, " \t");
            }
        }

        if (r->nb_hypotheses == 0) {
            liberer_regle(r);
            continue;
        }

        /* conclusion : entre "->" et ";" */
        conc = fleche + 2;
        if (semi != NULL) *semi = '\0';
        conc = trim_str(conc);

        if (conc[0] == '\0') {
            liberer_regle(r);
            continue;
        }

        strncpy(r->conclusion, conc, MAX_NOM - 1);
        r->conclusion[MAX_NOM - 1] = '\0';

        ajouter_regle_base(br, r);
        nb++;
    }

    fclose(fp);
    printf("  %d regle(s) chargee(s) depuis '%s'\n", nb, fichier);
    return 1;
}

int sauvegarder_base_regles(const BaseRegles *br, const char *fichier)
{
    FILE *fp;
    const Regle *r;
    const Litteral *h;

    if (br == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "w");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible de creer '%s'\n", fichier);
        return 0;
    }

    fprintf(fp, "# Base de regles\n");
    r = br->regles;
    while (r != NULL) {
        h = r->hypotheses;
        while (h != NULL) {
            fprintf(fp, "%s ", h->nom);
            h = h->suivant;
        }
        fprintf(fp, "-> %s ;\n", r->conclusion);
        r = r->suivant;
    }

    fclose(fp);
    printf("  Base de regles sauvegardee dans '%s'\n", fichier);
    return 1;
}

/* ================================================================== */
/*  FAITS                                                              */
/* ================================================================== */

int charger_base_faits(BaseFaits *bf, const char *fichier)
{
    FILE *fp;
    char ligne[256];
    int nb = 0;

    if (bf == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "r");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible d'ouvrir '%s'\n", fichier);
        return 0;
    }

    while (fgets(ligne, sizeof(ligne), fp) != NULL) {
        char *p;
        char *semi;

        p = trim_str(ligne);
        if (p[0] == '#' || p[0] == '\0')
            continue;

        semi = strchr(p, ';');
        if (semi != NULL) *semi = '\0';
        p = trim_str(p);

        if (p[0] == '\0') continue;

        if (ajouter_fait(bf, p))
            nb++;
    }

    fclose(fp);
    printf("  %d fait(s) charge(s) depuis '%s'\n", nb, fichier);
    return 1;
}

int sauvegarder_base_faits(const BaseFaits *bf, const char *fichier)
{
    FILE *fp;
    const Fait *f;

    if (bf == NULL || fichier == NULL) return 0;

    fp = fopen(fichier, "w");
    if (fp == NULL) {
        fprintf(stderr, "  Erreur : impossible de creer '%s'\n", fichier);
        return 0;
    }

    fprintf(fp, "# Base de faits\n");
    f = bf->faits;
    while (f != NULL) {
        fprintf(fp, "%s ;\n", f->nom);
        f = f->suivant;
    }

    fclose(fp);
    printf("  Base de faits sauvegardee dans '%s'\n", fichier);
    return 1;
}
