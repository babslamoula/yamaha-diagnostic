# Yamaha Diagnostic - Systeme Expert

Systeme expert de diagnostic de pannes pour motos Yamaha (plateformes CP2 / CP3 / CP4).
Projet S6 - ISEN P23.

## Compilation

```bash
make            # build release (-O2)
make debug      # build debug + ASAN/UBSAN
make test       # tests fumee (regressions audit P0)
make clean
```

## Lancement

```bash
./yamaha_diag                       # bases dans ./data par defaut
./yamaha_diag --data /chemin/data   # repertoire de bases personnalise
./yamaha_diag --help                # toutes les options
./yamaha_diag --version
```

## Structure

```
src/        sources C
include/    headers
data/       bases de regles, solutions et questions
tests/      tests fumee (smoke.sh)
```

## Fonctionnalites

- Diagnostic guide (mode Akinator) : selection modele -> generation -> questions symptomes -> panne identifiee + solution + cout estime
- Chainage avant : deduit tous les faits a partir d'une base de faits
- Chainage arriere : verifie un but precis (avec detection de cycle pour eviter les boucles infinies dans la base)
- Gestion manuelle des regles et faits
- Chargement / sauvegarde de bases au format texte
- Validation au chargement (auto-reference, cycles directs, litteraux orphelins)

## Modeles supportes

**CP4** : YZF-R1, MT-10

**CP2** : MT-07, Tracer 7 (le Tracer 700 et Tracer 7 partagent le meme moteur CP2 ; les diagnostics specifiques visent le Tracer 7 a partir de 2020), Tenere 700, XSR 700

**CP3** : MT-09, Tracer 900, XSR 900, Niken

## Format des fichiers de base

### `yamaha_regles.txt`

```
hypothese1 hypothese2 ... -> conclusion ;
```

Une ligne par regle. Lignes commencant par `#` ignorees.

### `yamaha_solutions.txt`

```
diag_id|nom|description|solution|cout_eur
```

### `yamaha_questions.txt`

```
litteral_id|question affichee
```

## Tests

`make test` execute `tests/smoke.sh` qui couvre les 3 regressions audit P0 :

- EOF sur stdin ne fait pas hang
- Cycle dans la base ne fait pas segfault
- Diagnostic plateforme l'emporte sur le diagnostic generique

## Audit

Voir [AUDIT.md](AUDIT.md) pour la liste complete des bugs identifies et leur statut.
