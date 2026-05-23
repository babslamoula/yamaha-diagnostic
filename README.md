# Yamaha Diagnostic - Systeme Expert

Systeme expert de diagnostic de pannes pour motos Yamaha (CP2 / CP3 / CP4).  
Projet S6 - ITII P23.

## Compilation

```bash
make
```

## Lancement

```bash
./yamaha_diag
```

Le programme cherche les bases de connaissance dans `./data/` au demarrage.

## Structure

```
src/        sources C
include/    headers
data/       bases de regles, solutions et questions
```

## Fonctionnalites

- Diagnostic guidé (mode Akinator) : selection modele → generation → questions symptomes → panne + solution + coût estimé
- Chainage avant et arriere
- Gestion manuelle des bases de regles et de faits
- Chargement / sauvegarde fichiers

## Modeles supportes

**CP4** : YZF-R1, MT-10  
**CP2** : MT-07, Tracer 700, Ténéré 700, XSR 700  
**CP3** : MT-09, Tracer 900, XSR 900, Niken
