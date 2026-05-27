# Graph Report - .  (2026-05-27)

## Corpus Check
- Corpus is ~9,908 words - fits in a single context window. You may not need a graph.

## Summary
- 196 nodes · 274 edges · 18 communities detected
- Extraction: 52% EXTRACTED · 48% INFERRED · 0% AMBIGUOUS · INFERRED: 132 edges (avg confidence: 0.83)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- [[_COMMUNITY_Coeur IO Bases Faits & Interface|Coeur IO Bases Faits & Interface]]
- [[_COMMUNITY_Arbre Demarrage + Specifiques Plateforme|Arbre Demarrage + Specifiques Plateforme]]
- [[_COMMUNITY_Diagnostic Demarreur|Diagnostic Demarreur]]
- [[_COMMUNITY_Partie Cycle & Freins|Partie Cycle & Freins]]
- [[_COMMUNITY_Gestion Base Regles & Fichiers|Gestion Base Regles & Fichiers]]
- [[_COMMUNITY_Moteur Roulant Bruit & Puissance|Moteur Roulant: Bruit & Puissance]]
- [[_COMMUNITY_Refroidissement & Surchauffe|Refroidissement & Surchauffe]]
- [[_COMMUNITY_Electrique HS BatterieFusible|Electrique HS: Batterie/Fusible]]
- [[_COMMUNITY_Securites Coupe-circuitBequilleCommodo|Securites: Coupe-circuit/Bequille/Commodo]]
- [[_COMMUNITY_Moteur d Inferences (Chainage)|Moteur d Inferences (Chainage)]]
- [[_COMMUNITY_Tests Theoriques (BNF Demo)|Tests Theoriques (BNF Demo)]]
- [[_COMMUNITY_Header solutions.h|Header solutions.h]]
- [[_COMMUNITY_Header fichiers.h|Header fichiers.h]]
- [[_COMMUNITY_Header interface.h|Header interface.h]]
- [[_COMMUNITY_Header moteur.h|Header moteur.h]]
- [[_COMMUNITY_Header base_regles.h|Header base_regles.h]]
- [[_COMMUNITY_Header base_faits.h|Header base_faits.h]]
- [[_COMMUNITY_Header structures.h|Header structures.h]]

## God Nodes (most connected - your core abstractions)
1. `main()` - 30 edges
2. `mode_diagnostic()` - 15 edges
3. `Arbre 4: Pannes Specifiques par Plateforme` - 11 edges
4. `verifier_but()` - 9 edges
5. `gerer_regles()` - 9 edges
6. `gerer_faits()` - 9 edges
7. `ajouter_fait()` - 8 edges
8. `charger_base_regles()` - 7 edges
9. `liberer_regle()` - 6 edges
10. `intermediate: elec_base_ok` - 6 edges

## Surprising Connections (you probably didn't know these)
- `Plateforme CP2 (MT-07, Tracer 700, Tenere 700, XSR 700)` --references--> `diag_bulle_vibration_tracer700`  [INFERRED]
  README.md → data/yamaha_regles.txt
- `Plateforme CP2 (MT-07, Tracer 700, Tenere 700, XSR 700)` --references--> `diag_rayons_oxydes_t7`  [INFERRED]
  README.md → data/yamaha_regles.txt
- `Plateforme CP3 (MT-09, Tracer 900, XSR 900, Niken)` --references--> `diag_aerodynamisme_tracer900_bagagerie`  [INFERRED]
  README.md → data/yamaha_regles.txt
- `mode_diagnostic()` --calls--> `moteur_reinit()`  [INFERRED]
  src/main.c → src/moteur.c
- `verifier_but()` --calls--> `trouver_question()`  [INFERRED]
  src/moteur.c → src/solutions.c

## Hyperedges (group relationships)
- **Diagnostic tree: probleme demarrage** — yamaha_questions_s_pb_demarrage, yamaha_regles_arbre_demarrage, yamaha_regles_elec_base_ok, yamaha_regles_elec_hs, yamaha_regles_secu_active_ou_commodo, yamaha_regles_demarreur_faible, yamaha_regles_demarreur_ok, yamaha_regles_diag_fusible_principal, yamaha_regles_diag_batterie_hs, yamaha_regles_diag_relais_demarreur_hs, yamaha_solutions_diag_fusible_principal, yamaha_solutions_diag_batterie_hs [EXTRACTED 0.95]
- **Diagnostic tree: surchauffe moteur** — yamaha_questions_s_surchauffe_moteur, yamaha_regles_pb_surchauffe, yamaha_regles_relais_ou_sonde_ventilo, yamaha_regles_diag_fuite_liquide_refroidissement, yamaha_regles_diag_pompe_a_eau_ou_calorstat_bloque, yamaha_regles_diag_fusible_ventilo, yamaha_regles_diag_relais_ou_sonde_temperature, yamaha_solutions_diag_fuite_liquide_refroidissement, yamaha_solutions_diag_pompe_a_eau_ou_calorstat_bloque [EXTRACTED 0.95]
- **Diagnostic tree: pb freinage et frein mou** — yamaha_questions_s_pb_freinage, yamaha_questions_s_levier_mou_ou_elastique, yamaha_regles_pb_freins, yamaha_regles_frein_mou, yamaha_regles_diag_purge_frein_necessaire, yamaha_regles_diag_plaquettes_tres_usees, yamaha_regles_diag_joints_etrier_hs, yamaha_regles_diag_disques_voiles_ou_plaquettes_glacees, yamaha_regles_diag_plaquettes_usees_metal [EXTRACTED 0.95]

## Communities

### Community 0 - "Coeur IO Bases Faits & Interface"
Cohesion: 0.12
Nodes (34): afficher_base_faits(), ajouter_fait(), copier_base_faits(), fait_existe(), init_base_faits(), liberer_base_faits(), supprimer_fait(), vider_base_faits() (+26 more)

### Community 1 - "Arbre Demarrage + Specifiques Plateforme"
Cohesion: 0.1
Nodes (27): Plateforme CP2 (MT-07, Tracer 700, Tenere 700, XSR 700), Plateforme CP3 (MT-09, Tracer 900, XSR 900, Niken), Plateforme CP4 (YZF-R1, MT-10), Yamaha Diagnostic - Systeme Expert (README), s_contact_mis: contact mis, s_pb_demarrage: probleme demarrage moto, Arbre 1: Problemes de Demarrage, Arbre 4: Pannes Specifiques par Plateforme (+19 more)

### Community 2 - "Diagnostic Demarreur"
Cohesion: 0.09
Nodes (22): s_action_de_demarrer: action demarrer, s_clac_clac_relais: clac-clac relais, s_demarreur_lent, s_demarreur_tourne_franchement, s_tableau_allume: tableau bord allume, intermediate: demarreur_faible, intermediate: demarreur_ok, diag_batterie_dechargee_ou_sulfatee (+14 more)

### Community 3 - "Partie Cycle & Freins"
Cohesion: 0.11
Nodes (19): s_levier_mou_ou_elastique, s_pb_freinage, s_pb_partie_cycle, Arbre 3: Chassis, Partie-cycle et Freinage, diag_amortisseur_arriere_fatigue, diag_disques_voiles_ou_plaquettes_glacees, diag_equilibrage_roue_ou_suspension, diag_joints_etrier_hs (+11 more)

### Community 4 - "Gestion Base Regles & Fichiers"
Cohesion: 0.17
Nodes (16): afficher_base_regles(), ajouter_regle_base(), creer_regle_interactive(), init_base_regles(), liberer_base_regles(), supprimer_regle(), charger_base_faits(), charger_base_regles() (+8 more)

### Community 5 - "Moteur Roulant: Bruit & Puissance"
Cohesion: 0.12
Nodes (17): s_bruit_surtout_a_froid, s_calage_au_retrogradage, s_cliquetis_metallique_haut, s_pb_moteur_roulant, Arbre 2: Comportement Moteur en Roulant, diag_capteur_tps_defaillant, diag_cloche_embrayage_marquee, diag_eau_dans_essence (+9 more)

### Community 6 - "Refroidissement & Surchauffe"
Cohesion: 0.13
Nodes (15): s_liquide_refroidissement_bas, s_liquide_refroidissement_ok, s_surchauffe_moteur, s_ventilateur_declenche, s_ventilateur_ne_declenche_pas, diag_fuite_liquide_refroidissement, diag_fusible_ventilo, diag_pompe_a_eau_ou_calorstat_bloque (+7 more)

### Community 7 - "Electrique HS: Batterie/Fusible"
Cohesion: 0.17
Nodes (12): s_fusible_principal_hs: fusible principal grille, s_fusible_principal_ok: fusible principal intact, s_tableau_eteint: tableau bord eteint, s_tension_batterie_basse: tension batterie < 12V, s_tension_batterie_ok: tension batterie > 12.3V, diag_batterie_hs, diag_faisceau_coupe_ou_neiman, diag_fusible_principal (+4 more)

### Community 8 - "Securites: Coupe-circuit/Bequille/Commodo"
Cohesion: 0.2
Nodes (10): s_rien_du_tout, diag_commodo_demarrage_encrasse, diag_erreur_bequille_vitesse, diag_erreur_coupe_circuit, diag_relais_demarreur_hs, intermediate: secu_active_ou_commodo, Commodo start dur ou grippe (30 EUR), Securite bequille enclenchee (15 EUR) (+2 more)

### Community 9 - "Moteur d Inferences (Chainage)"
Cohesion: 0.43
Nodes (7): chainage_arriere(), chainage_arriere_continu(), deja_pose(), marquer_pose(), moteur_reinit(), tenter_regles(), verifier_but()

### Community 10 - "Tests Theoriques (BNF Demo)"
Cohesion: 1.0
Nodes (2): Test facts: B, C, Test rules: synthetic propositional rules

### Community 11 - "Header solutions.h"
Cohesion: 1.0
Nodes (0): 

### Community 12 - "Header fichiers.h"
Cohesion: 1.0
Nodes (0): 

### Community 13 - "Header interface.h"
Cohesion: 1.0
Nodes (0): 

### Community 14 - "Header moteur.h"
Cohesion: 1.0
Nodes (0): 

### Community 15 - "Header base_regles.h"
Cohesion: 1.0
Nodes (0): 

### Community 16 - "Header base_faits.h"
Cohesion: 1.0
Nodes (0): 

### Community 17 - "Header structures.h"
Cohesion: 1.0
Nodes (0): 

## Knowledge Gaps
- **70 isolated node(s):** `diag_roulement_boite_ou_embrayage`, `diag_cloche_embrayage_marquee`, `diag_capteur_tps_defaillant`, `diag_eau_dans_essence`, `diag_filtre_pompe_bouche` (+65 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **Thin community `Tests Theoriques (BNF Demo)`** (2 nodes): `Test facts: B, C`, `Test rules: synthetic propositional rules`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header solutions.h`** (1 nodes): `solutions.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header fichiers.h`** (1 nodes): `fichiers.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header interface.h`** (1 nodes): `interface.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header moteur.h`** (1 nodes): `moteur.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header base_regles.h`** (1 nodes): `base_regles.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header base_faits.h`** (1 nodes): `base_faits.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.
- **Thin community `Header structures.h`** (1 nodes): `structures.h`
  Too small to be a meaningful cluster - may be noise or needs more connections extracted.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Arbre 1: Problemes de Demarrage` connect `Arbre Demarrage + Specifiques Plateforme` to `Diagnostic Demarreur`, `Electrique HS: Batterie/Fusible`?**
  _High betweenness centrality (0.083) - this node is a cross-community bridge._
- **Why does `intermediate: elec_base_ok` connect `Diagnostic Demarreur` to `Securites: Coupe-circuit/Bequille/Commodo`, `Arbre Demarrage + Specifiques Plateforme`?**
  _High betweenness centrality (0.082) - this node is a cross-community bridge._
- **Why does `main()` connect `Coeur IO Bases Faits & Interface` to `Moteur d Inferences (Chainage)`, `Gestion Base Regles & Fichiers`?**
  _High betweenness centrality (0.063) - this node is a cross-community bridge._
- **Are the 26 inferred relationships involving `main()` (e.g. with `init_base_regles()` and `init_base_faits()`) actually correct?**
  _`main()` has 26 INFERRED edges - model-reasoned connections that need verification._
- **Are the 12 inferred relationships involving `mode_diagnostic()` (e.g. with `init_base_faits()` and `copier_base_faits()`) actually correct?**
  _`mode_diagnostic()` has 12 INFERRED edges - model-reasoned connections that need verification._
- **Are the 3 inferred relationships involving `verifier_but()` (e.g. with `fait_existe()` and `trouver_question()`) actually correct?**
  _`verifier_but()` has 3 INFERRED edges - model-reasoned connections that need verification._
- **Are the 7 inferred relationships involving `gerer_regles()` (e.g. with `effacer_ecran()` and `menu_gestion_regles()`) actually correct?**
  _`gerer_regles()` has 7 INFERRED edges - model-reasoned connections that need verification._