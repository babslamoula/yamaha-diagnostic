---
type: community
cohesion: 0.13
members: 15
---

# Refroidissement & Surchauffe

**Cohesion:** 0.13 - loosely connected
**Members:** 15 nodes

## Members
- [[Calorstat soude ou turbine HS (250 EUR)]] - document - data/yamaha_solutions.txt
- [[Fuite LDR (50 EUR)]] - document - data/yamaha_solutions.txt
- [[Fusible ventilo mort (30 EUR)]] - document - data/yamaha_solutions.txt
- [[Sonde CTS HS  Relais OMRON (150 EUR)]] - document - data/yamaha_solutions.txt
- [[diag_fuite_liquide_refroidissement]] - document - data/yamaha_regles.txt
- [[diag_fusible_ventilo]] - document - data/yamaha_regles.txt
- [[diag_pompe_a_eau_ou_calorstat_bloque]] - document - data/yamaha_regles.txt
- [[diag_relais_ou_sonde_temperature]] - document - data/yamaha_regles.txt
- [[intermediate pb_surchauffe]] - document - data/yamaha_regles.txt
- [[intermediate relais_ou_sonde_ventilo]] - document - data/yamaha_regles.txt
- [[s_liquide_refroidissement_bas]] - document - data/yamaha_questions.txt
- [[s_liquide_refroidissement_ok]] - document - data/yamaha_questions.txt
- [[s_surchauffe_moteur]] - document - data/yamaha_questions.txt
- [[s_ventilateur_declenche]] - document - data/yamaha_questions.txt
- [[s_ventilateur_ne_declenche_pas]] - document - data/yamaha_questions.txt

## Live Query (requires Dataview plugin)

```dataview
TABLE source_file, type FROM #community/Refroidissement_&_Surchauffe
SORT file.name ASC
```

## Connections to other communities
- 1 edge to [[_COMMUNITY_Moteur Roulant Bruit & Puissance]]

## Top bridge nodes
- [[intermediate pb_surchauffe]] - degree 6, connects to 1 community