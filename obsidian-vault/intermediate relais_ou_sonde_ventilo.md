---
source_file: "data/yamaha_regles.txt"
type: "document"
community: "Refroidissement & Surchauffe"
tags:
  - graphify/document
  - graphify/EXTRACTED
  - community/Refroidissement_&_Surchauffe
---

# intermediate: relais_ou_sonde_ventilo

## Connections
- [[diag_fusible_ventilo]] - `implements` [EXTRACTED]
- [[diag_relais_ou_sonde_temperature]] - `implements` [EXTRACTED]
- [[intermediate pb_surchauffe]] - `implements` [EXTRACTED]
- [[s_ventilateur_ne_declenche_pas]] - `references` [INFERRED]

#graphify/document #graphify/EXTRACTED #community/Refroidissement_&_Surchauffe