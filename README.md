# AbsolutOrdor - Projet L3 Informatique CY Paris Université

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)


Un projet en C d'ordonnacement algorithmique et de simulation du comportement d'un processeur.

---

## Utilisation


Notre application possède 2 modes de fonctionnements.


### Mode CLI

- **-f, -filepath <path>**  :  Fichier CSV des processus
- **-i, -inline   <str>**  :   Contenu CSV passe directement en argument
- **-s, -select   <algo>**  :  Algorithme a utiliser (ex: FIFO, SJF, all pour tous ...)
- **-n <quantum>** : Quantum si vous utilisez RR (defaut: 2)\n"
- **-o, -output   <path>** : Fichier de sortie CSV des resultats
- **-h, -help** : Affiche les commandes
- **-f ou -i sont obligatoire**, il servent à indiquer les données passé au système. Les processus qui auront lieu et qu'il faudra ordonnancer.
- **-s** Sert à sélectionner un algorithme parmis
    - FIFO
    - SJF
    - SJRF
    - RR (paramètre **-n** requis)
    - LOTTERY
    - all

#### Exemple

```sh
 - CLI.exe -f processus.csv -s FIFO\n"
 - CLI.exe -f processus.csv -s RR -n 3 -o resultats.csv\n"
 - CLI.exe -i \"2\\nP1;0;5;3\\nP2;2;3\" -s SJF (Pour un csv inline)
 - CLI.exe -f processus.csv -s all -o resultats.csv\n"
```

```sh
 CLI.exe -f <fichier.csv> -s <ALGO> 
 CLI.exe -i <contenu.csv> -s <ALGO>
 CLI.exe -f <fichier.csv> -s all
 CLI.exe -f <fichier.csv> -s FIFO -o <sortie.csv>
```


### Mode GUI

Interface temporaire



### Format fichier CSV

A partir de la 3 ème valeur, on alterne entre UC et ES. On simplifie notre système en disant qu'un programme n'est finalement qu'une alternance variant dans le temps entre les deux.

```csv
#ProcessName;DateApparition;Nb_UC1;Nb_ES1;...;Nb_UCN;Nb_ESN
6
# Jeu de test complet pour l’ordonnanceur
Process1;0;5;5;9;6;2;4
Process2;2;8;3;4
Process3;4;3;2
Process4;1;6;4;3;5
Process5;3;7;2
Process6;5;2;6;3
```



### Installation

```sh
git clone https://github.com/GauthierDefrance/AbsolutOrdor/
cmake --build build
