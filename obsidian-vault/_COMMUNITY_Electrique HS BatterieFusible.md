---
type: community
cohesion: 0.17
members: 12
---

# Electrique HS: Batterie/Fusible

**Cohesion:** 0.17 - loosely connected
**Members:** 12 nodes

## Members
- [[Batterie completement detruite (120 EUR)]] - document - data/yamaha_solutions.txt
- [[Faisceau sectionne ou Neiman mort (150 EUR)]] - document - data/yamaha_solutions.txt
- [[Fusible principal 3050A grille (25 EUR)]] - document - data/yamaha_solutions.txt
- [[diag_batterie_hs]] - document - data/yamaha_regles.txt
- [[diag_faisceau_coupe_ou_neiman]] - document - data/yamaha_regles.txt
- [[diag_fusible_principal]] - document - data/yamaha_regles.txt
- [[intermediate elec_hs]] - document - data/yamaha_regles.txt
- [[s_fusible_principal_hs fusible principal grille]] - document - data/yamaha_questions.txt
- [[s_fusible_principal_ok fusible principal intact]] - document - data/yamaha_questions.txt
- [[s_tableau_eteint tableau bord eteint]] - document - data/yamaha_questions.txt
- [[s_tension_batterie_basse tension batterie  12V]] - document - data/yamaha_questions.txt
- [[s_tension_batterie_ok tension batterie  12.3V]] - document - data/yamaha_questions.txt

## Live Query (requires Dataview plugin)

```dataview
TABLE source_file, type FROM #community/Electrique_HS:_Batterie/Fusible
SORT file.name ASC
```

## Connections to other communities
- 1 edge to [[_COMMUNITY_Arbre Demarrage + Specifiques Plateforme]]

## Top bridge nodes
- [[intermediate elec_hs]] - degree 6, connects to 1 community