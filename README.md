# Yamaha Diagnostic - Systeme expert

Projet ISENA3 - Techniques de Programmation / Intelligence Artificielle  
Groupe : Baptiste, Isaie, Lucas

Yamaha Diagnostic est une application console en C qui aide a diagnostiquer des pannes de motos Yamaha sur les plateformes CP2, CP3 et CP4. Le programme utilise une base de faits, une base de regles et un moteur d'inference pour proposer un diagnostic, une solution et un cout estime.

## Fonctionnalites

- Diagnostic guide par questions oui/non.
- Selection du modele Yamaha et de la generation.
- Chainage avant pour deduire toutes les pannes possibles.
- Chainage arriere pour verifier un diagnostic cible.
- Gestion manuelle des faits et des regles.
- Chargement et sauvegarde de bases texte.
- Validation de la base de regles au chargement.
- Documentation Doxygen.
- Tests fumee et tests unitaires de modules.

## Prerequis

- `make`
- `gcc` ou `clang`
- optionnel : `doxygen` pour generer la documentation HTML

Le projet n'utilise pas de bibliotheque externe obligatoire.

## Compilation

```sh
make
```

Compilation debug avec ASAN/UBSAN :

```sh
make debug
```

Nettoyage :

```sh
make clean
```

## Lancement

```sh
./yamaha_diag
```

Options disponibles :

```sh
./yamaha_diag --help
./yamaha_diag --version
./yamaha_diag --data ./data
```

## Tests

```sh
make test
```

La cible lance :

- `tests/smoke.sh` pour les regressions critiques ;
- `tests/Makefile` pour les tests unitaires C.

Derniere validation avant rendu :

```text
Tests fumee : 3 pass / 0 fail
Tests unitaires : 7 pass / 0 fail
```

Les tests couvrent notamment :

- entree standard fermee sans boucle infinie ;
- cycle dans la base de regles sans crash ;
- priorite des diagnostics Yamaha specifiques sur les diagnostics generiques ;
- chargement/sauvegarde de faits et regles ;
- structures, listes chainees, moteur, solutions et interface.

## Documentation

```sh
make docs
```

La documentation HTML est generee dans :

```text
docs/html/index.html
```

Nettoyage de la documentation generee :

```sh
make clean-docs
```

## Arborescence

```text
src/        sources C
include/    headers
data/       bases de regles, questions, solutions et jeux de test
tests/      tests unitaires et tests fumee
docs/       source Doxygen et documentation generee
rendu/      rapport, cahier de tests, guide et presentation
```

## Donnees metier

La base Yamaha contient :

- 64 regles ;
- 81 questions ;
- 47 solutions de diagnostic.

Modeles couverts :

- CP4 : YZF-R1, MT-10 ;
- CP2 : MT-07, Tracer 7, Tenere 700, XSR 700 ;
- CP3 : MT-09, Tracer 900, XSR 900, Niken.

## Formats de fichiers

Regles :

```text
hypothese1 hypothese2 -> conclusion ;
```

Faits :

```text
fait ;
```

Solutions :

```text
diag_id|nom|description|solution|cout_eur
```

Questions :

```text
litteral_id|question affichee
```

## Livrables de rendu

Le dossier `rendu/` contient :

- `rapport_yamaha_diagnostic_ISENA3.md` et `.docx` ;
- `cahier_tests_yamaha_diagnostic.md` et `.docx` ;
- `guide_installation_utilisation.md` et `.docx` ;
- `presentation_yamaha_diagnostic_ISENA3.html` ;
- `presentation_yamaha_diagnostic_ISENA3.md` et `.docx` ;
- `script_video_presentation.md` ;
- `checklist_rendu.md`.

Ne pas rendre les artefacts generes :

- `yamaha_diag`
- `obj/`
- `tests/bin/`
- `.idea/`
- `.DS_Store`

## Audit technique

Le fichier `AUDIT.md` decrit les anomalies trouvees pendant la fiabilisation et leur statut.

