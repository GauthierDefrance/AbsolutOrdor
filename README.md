# AbsolutOrdor - Projet L3 Informatique CY Paris Université

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)


Un projet en C/C++ d'ordonnacement algorithmique et de simulation du comportement d'un processeur.

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



## Installation (Ubuntu)

Afin de compiler ce projet, vous aurez besoin de Cmake, openGL, 

```sh
#!/bin/bash

# D'abord on clone le github
git clone https://github.com/GauthierDefrance/AbsolutOrdor/

# Ensuite, on se rend dans le répertoire du projet
cd ./AbsolutOrdor

# Une fois dans le répertoire, on pourra utiliser cmake pour préparer à la compilation
cmake -S . -B build

# Enfin on compile réellement
cmake --build build

# Une fois tout cela fait, vous pouvez vous rendre dans le répertoire de build
cd ./build

# Dedans vous pourrez éxécuter vos programmes avec par exemples
./GUI
./CLI
```

#### En cas de problèmes lors de l'installation

Dans le cas ou vous rencontrez des problèmes lors de l'installation, il est probable que cela soit dû car votre machine ne possède pas certaines librairies.
Voici un script généraliste sur Ubuntu qui devrait résoudre ces problèmes là.

```sh
#!/bin/bash
# Script pour installer les dépendances OpenGL + GLFW sur Ubuntu

echo "Mise à jour des paquets..."
sudo apt update && sudo apt upgrade -y

echo "Installation des dépendances..."
sudo apt install -y \
    libxi-dev \
    libxcursor-dev \
    libxinerama-dev \
    libglfw3 \
    libglfw3-dev \
    libxrandr-dev \
    build-essential \
    mesa-utils \
    libglu1-mesa-dev \
    freeglut3-dev \
    cmake \
    git \
    pkg-config

echo "Installation terminée !"
```

#### Installation sur Windows

La portabilité vers Windows à été testé et fonctionne pour Windows10/11.
Cela dit vous aurez besoin d'installer certains modules comme Cmake ou un compilateur.

```sh
git clone https://github.com/GauthierDefrance/AbsolutOrdor/
cd ./AbsolutOrdor

# On utilise Ninja, il nous permet de compiler ce projet. D'autres compilateur comme GCC semble avoir du mal
cmake -S . -B build -G Ninja

cmake --build build

cd ./build

./GUI.exe
./CLI.exe
```

#### Installation sur MAC

La portabilité sur Mac n'a pas était testé.
Mais en installant Cmake et les dépendances qu'il demande,
il vous suffira de suivre des procédures relativement similaire à celui de l'installation sur Ubuntu.


## Librairies utilisées

- [ImGui](https://github.com/ocornut/imgui)  
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)  
- [ImSequencer](https://github.com/CedricGuillemet/ImGuizmo/blob/master/ImSequencer.h)

