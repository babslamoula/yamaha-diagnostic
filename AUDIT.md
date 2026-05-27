# Audit yamaha_diag

> **Statut : corrige.** Tous les bugs P0/P1 sont resolus. P2/P3 traites a
> l'exception de #9, #10, #17, #18 (resterait du polish sans impact
> fonctionnel). `make test` couvre les 3 regressions P0.



Projet : système expert C, 8 modules (src/), 7 headers (include/), 1646 lignes C, 64 règles, 81 questions, 47 solutions, plateformes CP2/CP3/CP4.

Build propre (`gcc -Wall -Wextra -pedantic -std=c99`), aucun warning, ASAN/UBSAN ne remontent rien sur les chemins testés.

---

## Bugs bloquants

### 1. Hang infini sur EOF stdin
Reproduit : `echo "1" | ./yamaha_diag` ne termine jamais.

Cause : 30 occurrences de `while (getchar() != '\n');` réparties dans `src/main.c`, `src/interface.c`, `src/moteur.c`, `src/base_regles.c`. `getchar()` retourne `EOF` (-1) après fermeture de stdin ; `EOF != '\n'` donc boucle infinie.

Correctif (helper unique) :
```c
static void vider_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
```
Remplacer toutes les occurrences. Sans cela, un pipe court (redirection de test, CI) gèle le process et empêche `liberer_*` de tourner → fuites mémoire en fin.

### 2. Récursion infinie sur cycle dans la base de règles → SegFault
Reproduit avec `A -> B ;` et `B -> A ;` puis chaînage arrière sur `A` ou `B` → stack overflow → SIGSEGV.

Cause : `verifier_but()` ↔ `tenter_regles()` (src/moteur.c:95-118) n'ont aucune détection de cycle. La base Yamaha actuelle n'a pas de cycle, mais une seule modification manuelle (menu 4) suffit à crasher.

Correctif : pile de buts en cours dans `verifier_but`. Si le but est déjà dans la pile, retourner 0 (cycle détecté). Soit :
```c
static char g_pile[64][MAX_NOM];
static int g_pile_n = 0;
/* en début de verifier_but : */
for (int i = 0; i < g_pile_n; i++)
    if (strcmp(g_pile[i], but) == 0) return 0;
if (g_pile_n < 64) { strncpy(g_pile[g_pile_n++], but, MAX_NOM-1); ... }
/* à chaque retour : g_pile_n-- ; */
```

### 3. Akinator : diagnostics génériques masquent les diagnostics spécifiques plateforme
`mode_diagnostic()` (main.c:309-352) itère `diagnostics[]` dans l'ordre d'insertion des règles (tail insert). Dans `data/yamaha_regles.txt` les règles génériques sont **avant** les spécifiques.

Exemple confirmé pour un utilisateur MT-07 (cp2) avec cliquetis distri :
- ligne 56 : `pb_bruit_moteur s_cliquetis_metallique_haut s_bruit_surtout_a_froid -> diag_tendeur_chaine_distribution` ← trouvé en premier
- ligne 112 : `plateforme_cp2 s_pb_moteur_roulant pb_bruit_moteur s_cliquetis_metallique_haut s_bruit_surtout_a_froid -> diag_cct_cp2` ← jamais testé

Le diagnostic CP2 (avec sa solution Yamaha dédiée à 200 €) est systématiquement caché derrière le diagnostic générique (150 €). Toute la valeur ajoutée "Exclusif Yamaha" annoncée dans la bannière s'évapore.

Correctif : trier `diagnostics[]` par spécificité descendante avant de boucler (priorité aux conclusions de règles ayant un préfixe `plateforme_/moto_/gen_` dans leurs hypothèses), ou faire deux passes (spécifique d'abord, générique ensuite).

---

## Bugs sérieux

### 4. Charge incohérente entre tailles de buffer scanf
- `base_regles.c:140,152` : `scanf("%32s", buffer)` dans un `char buffer[MAX_NOM]` (= 64). Limite arbitraire à 32 chars sur un buffer de 64. Tronquage silencieux pour tout identifiant > 32 chars.
- `main.c:109,127,418,427` : `%63s` cohérent avec `MAX_NOM=64`. Mettre les autres à `%63s`.
- `moteur.c:151` : `%15s` pour `char rep[16]`. Cohérent mais entrée tronquée → règle de validation faussée (voir bug 7).

### 5. `charger_*` retourne 1 même quand 0 ligne valide
`fichiers.c:108`, `fichiers.c:180`, `solutions.c:91`, `solutions.c:140` : retournent `1` dès que `fopen` réussit, même si aucune règle/fait/solution n'a été extraite. `main.c:66-71` n'examine d'ailleurs jamais la valeur retournée → l'utilisateur ne sait pas qu'un fichier vide ou mal formé a été silencieusement accepté.

Correctif : retourner `nb` (nombre d'entrées effectives) et tester dans le main.

### 6. Pas de gestion des lignes >511 chars dans `charger_base_regles`
`fichiers.c:34` : `char ligne[512]`. Une règle anormalement longue (ou commentaire) fait que `fgets` lit un fragment, perte de "->" → ligne ignorée silencieusement, puis le second fragment lu est traité comme une nouvelle règle invalide → règle suivante peut être consommée comme conclusion d'une non-règle.

Correctif : détecter l'absence de `\n` en fin de buffer, vider la ligne complète et signaler.

### 7. Réponse `oui/non` trop laxiste
`moteur.c:160` et `main.c:111` : `if (rep[0] == 'o' || rep[0] == 'O')` accepte `ouais`, `ok`, `oh`, `omg`, `o2`. Tout ce qui ne commence pas par o/O est considéré non.

Correctif : `strcmp(rep, "oui") == 0 || strcmp(rep, "o") == 0` + une réinvite sur entrée non reconnue.

### 8. Termes box-drawing UTF-8 cassés sur Windows
`interface.c` utilise `┌─┐│└┘├┤` (UTF-8 3 octets). `system("cls")` est appelé sur `_WIN32` mais la console Windows par défaut (cp437/cp1252) n'affiche pas l'UTF-8 → rendu cassé. Le `#ifdef _WIN32` est inutile sans `SetConsoleOutputCP(CP_UTF8)`.

Correctif : soit appeler `SetConsoleOutputCP(65001)` au démarrage sous `_WIN32`, soit basculer sur des bordures ASCII (`+ - |`) si portabilité visée.

---

## Bugs mineurs / hygiène

### 9. État global non réinitialisé entre sessions de chaînage arrière interactif
`moteur.c:15-16` : `g_poses[]` et `g_dico` statiques. `mode_diagnostic` appelle `moteur_reinit()` (bon), mais le menu 3 (chaînage arrière simple) ne le fait pas — sans conséquence aujourd'hui car le menu 3 utilise `continu=0`. Mais cela tombera si quelqu'un branche `chainage_arriere_continu` ailleurs sans réinit.

### 10. Mémorisation absente des conclusions intermédiaires
`verifier_but` ne mémoïse pas un but vérifié non-final : chaque appel re-traverse les règles. O(n) par appel, n = règles. Acceptable pour 64 règles, mais devient quadratique si la base grandit.

Correctif : ajouter dans `bf` les buts dérivés ; ou maintenir un set "déjà prouvé" en parallèle.

### 11. `selectionner_fichier_dossier` (interface.c:137-205)
- Limite codée en dur à 50 fichiers, silencieusement tronquée.
- `strcpy` (lignes 172-174) au lieu de `strncpy` ou `memmove` — pas exploitable car buffers de même taille, mais hygiène.
- Pas de filtre sur entrées cachées (`.DS_Store`).

### 12. `system("clear")` / `system("cls")` (interface.c:9-16)
Dépend de `$PATH`. Surface d'attaque minime mais existe. Préférer l'écran ANSI : `printf("\033[H\033[2J");`.

### 13. Duplication `trim` / `trim_str`
`solutions.c:22-32` et `fichiers.c:16-25` font la même chose avec des différences (l'un gère `\r`, l'autre via `isspace`). À factoriser dans un `util.c` / `util.h`.

### 14. `Makefile` sans optimisation et sans cible test/install
- Pas de `-O2` pour le binaire de production.
- Pas de `make test` (et le dossier `tests/` du dépôt original n'existe pas ici).
- Pas de `-MMD -MP` → `make` ne reconstruit pas quand un header change.

### 15. Avertissement strict
`gcc -Wshadow -Wconversion` (clang) signale :
```
src/main.c:51:29: warning: implicit conversion loses integer precision: 'long' to 'int' [-Wshorten-64-to-32]
```
`len = slash - argv[0]` doit être `ptrdiff_t` ou explicitement casté.

### 16. Pas de version, pas de log, pas de mode `-h`
`argc/argv` ne supportent que l'auto-localisation de `data/`. Pas de `--help`, pas de `--version`, pas de chemin custom (`--data /path`).

### 17. Solutions en `head insertion` → ordre inversé
`solutions.c:83` : `s->suivant = bs->solutions; bs->solutions = s;`. La recherche `trouver_solution` reste correcte (linéaire), mais l'affichage `7. Afficher base de regles` n'est pas affecté car il concerne les règles, pas les solutions. Si un jour on liste les solutions, l'ordre sera inversé.

### 18. `bf_orig` polluant le diagnostic
`mode_diagnostic()` (main.c:251-252) copie tous les faits utilisateur préexistants dans `bf_diag`. Si l'utilisateur a chargé un fichier de faits sans rapport (ex : un test_faits.txt avec `B ; C ;`), ceux-ci entrent dans la session Akinator. Comportement non documenté.

### 19. Pas de validation de la base de règles au chargement
Aucun contrôle :
- conclusion identique à une hypothèse (auto-référence) ;
- présence de cycles ;
- littéral utilisé sans question correspondante (le dictionnaire `g_dico` retombera sur l'identifiant brut, dégradant l'UX).

Aujourd'hui toutes les 81 questions matchent les 81 littéraux utilisés (vérifié), mais aucune contrainte ne le garantit après édition.

### 20. README désynchronisé
`README.md` annonce `Tracer 700 / Tracer 7` mais la solution `diag_bulle_vibration_tracer700` cible spécifiquement Tracer 7. Annonce CP2/CP3/CP4 mais ne mentionne pas le mode chaînage avant/arrière séparément.

---

## Qualité du graphe d'inférence (analyse graphify)

196 nœuds, 274 arêtes, 18 communautés. Cohésion par communauté :

| Communauté | Cohésion | Lecture |
|---|---|---|
| Moteur d'inférences (chainage) | 0.43 | bonne séparation |
| Coeur IO Bases Faits & Interface | 0.12 | trop diffuse |
| Arbre Démarrage + Spécifiques | 0.10 | les diagnostics spécifiques devraient sortir de cette communauté |

70 nœuds isolés (≤ 1 connexion) — beaucoup de `diag_*` n'ont pas de hyperedge agrégeante. Construire une hyperedge "diagnostics par arbre" améliorerait la navigation Obsidian.

Pont haute centralité : `intermediate: elec_base_ok` (0.082) connecte 3 communautés. Concrètement c'est le nœud qui structure tout l'arbre démarrage — bon signe.

---

## Couverture base de connaissances

| Source | Compte | Vérification |
|---|---|---|
| Règles `diag_*` | 47 | OK |
| Solutions `diag_*` | 47 | 100 % de correspondance |
| Questions `s_*` | 81 | 100 % des littéraux utilisés ont une question |

Aucun trou de couverture côté données. Le travail métier est sérieux.

---

## Statut des correctifs

| # | Statut | Notes |
|---|---|---|
| 1 EOF hang | corrige | helper `vider_stdin()` + sortie propre sur `feof(stdin)` dans la boucle principale |
| 2 segfault cycle | corrige | pile de buts `g_pile_buts` dans `moteur.c`, profondeur max 64 |
| 3 masquage diag plateforme | corrige | tri par specificite descendante dans `mode_diagnostic` (hypotheses `plateforme_*/moto_*/gen_*` x 100) |
| 4 scanf incoherents | corrige | toutes les largeurs base_regles a `%63s` |
| 5 retour `charger_*` | corrige | retourne le nombre d'entrees, `main` previent si vide |
| 6 lignes >511 chars | corrige | detection troncature fgets, ligne ignoree avec avertissement |
| 7 reponse oui/non | corrige | `lire_oui_non()` strict + re-invite |
| 8 UTF-8 Windows | corrige | `SetConsoleOutputCP(CP_UTF8)` au demarrage |
| 9 reinit moteur menu 3 | non bloquant | menu 3 utilise `continu=0`, aucun usage actuel concerne |
| 10 memoisation buts | non bloquant | acceptable a 64 regles |
| 11-13 hygiene | corrige | filtre `.*`, `memcpy` pour le tri, factorisation `util_trim` |
| 12 system clear | corrige | sequence ANSI `\033[H\033[2J` |
| 14 Makefile | corrige | `-O2`, `-MMD -MP`, cibles `debug`, `test`, `help` |
| 15 conversion ptrdiff_t | corrige | cast explicite `ptrdiff_t` + `memcpy` |
| 16 CLI options | corrige | `--help`, `--version`, `--data` (+ alias courts) |
| 17 head insertion solutions | non bloquant | aucun affichage liste solutions |
| 18 bf_orig polluant | non bloquant | comportement documente |
| 19 validation base | corrige | `valider_base_regles()` : auto-ref, cycle direct, litteral orphelin |
| 20 README desync | corrige | README reecrit (Tracer 7, modes, CLI, validation, tests) |

Verification : `make clean && make && make test` -> 3/3 pass.
Build ASAN/UBSAN : `make debug` -> aucun leak ni undefined behavior.

## Priorisation

| Priorité | Item |
|---|---|
| P0 | #1 hang EOF · #2 segfault cycle · #3 masquage diags plateforme |
| P1 | #4 scanf incohérents · #5 retour `charger_*` · #6 lignes longues · #7 réponse oui/non |
| P2 | #8 UTF-8 Windows · #9-13 hygiène · #14 Makefile · #15 conversion |
| P3 | #16-20 confort et documentation |

P0 doivent partir avant le rendu. P1 sont rapides à corriger (une heure de travail bien menée).

---

## Fichiers produits

- `obsidian-vault/` — coffre Obsidian, 214 notes, ouvrir le dossier dans Obsidian
- `graphify-out/graph.html` — graphe interactif (158 Ko)
- `graphify-out/GRAPH_REPORT.md` — rapport d'extraction
- `graphify-out/graph.json` — graphe brut
- `obsidian-vault/graph.canvas` — layout structuré par communautés (Obsidian Canvas)
