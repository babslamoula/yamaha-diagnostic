/* util.h - helpers partages (entrees, chaines, ecran)
 *
 * Centralise les helpers qui etaient duplicates/cassants dans le projet :
 *   - vider_stdin : remplace les "while (getchar() != '\n');" qui boucle
 *     a l'infini sur EOF (audit P0 #1)
 *   - lire_oui_non : remplace les tests "rep[0] == 'o'" trop laxistes
 *     (audit P1 #7)
 *   - trim : factorisation des deux versions divergentes de trim_str /
 *     trim presentes dans fichiers.c et solutions.c (audit P2 #13)
 */

#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/* Lit et jette les caracteres jusqu'au '\n' OU EOF. Ne boucle jamais. */
void vider_stdin(void);

/* Demande une reponse oui/non a l'utilisateur, re-invite tant que ce n'est
 * pas reconnu. Retourne 1 pour oui, 0 pour non, -1 si EOF stdin (le code
 * appelant doit alors abandonner l'interaction au lieu de spinner). */
int lire_oui_non(const char *invite);

/* Supprime espaces, tabs, \r et \n en debut et fin. Retourne un pointeur
 * dans la meme chaine (peut etre != s si trim debut). Modifie 's' in-place. */
char *util_trim(char *s);

/* Initialise la console (UTF-8 sous Windows). A appeler une fois au demarrage
 * du programme avant tout printf utilisant des box-drawings. */
void util_init_console(void);

#endif /* UTIL_H */
