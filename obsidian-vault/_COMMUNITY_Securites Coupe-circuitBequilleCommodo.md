---
type: community
cohesion: 0.20
members: 10
---

# Securites: Coupe-circuit/Bequille/Commodo

**Cohesion:** 0.20 - loosely connected
**Members:** 10 nodes

## Members
- [[Commodo start dur ou grippe (30 EUR)]] - document - data/yamaha_solutions.txt
- [[Coupe-circuit enclenche (0 EUR)]] - document - data/yamaha_solutions.txt
- [[Relais demarreur grille (80 EUR)]] - document - data/yamaha_solutions.txt
- [[Securite bequille enclenchee (15 EUR)]] - document - data/yamaha_solutions.txt
- [[diag_commodo_demarrage_encrasse]] - document - data/yamaha_regles.txt
- [[diag_erreur_bequille_vitesse]] - document - data/yamaha_regles.txt
- [[diag_erreur_coupe_circuit]] - document - data/yamaha_regles.txt
- [[diag_relais_demarreur_hs]] - document - data/yamaha_regles.txt
- [[intermediate secu_active_ou_commodo]] - document - data/yamaha_regles.txt
- [[s_rien_du_tout]] - document - data/yamaha_questions.txt

## Live Query (requires Dataview plugin)

```dataview
TABLE source_file, type FROM #community/Securites:_Coupe-circuit/Bequille/Commodo
SORT file.name ASC
```

## Connections to other communities
- 1 edge to [[_COMMUNITY_Diagnostic Demarreur]]

## Top bridge nodes
- [[intermediate secu_active_ou_commodo]] - degree 6, connects to 1 community