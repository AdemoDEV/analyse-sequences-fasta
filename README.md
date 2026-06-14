# Analyse de séquences FASTA

![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CSV](https://img.shields.io/badge/CSV-217346?style=for-the-badge&logo=microsoftexcel&logoColor=white)
![FASTA](https://img.shields.io/badge/FASTA-4B8BBE?style=for-the-badge)

## Description

Projet réalisé dans le cadre du cours **INFO-F-105 — Langages de programmation 1** à l’Université Libre de Bruxelles.

Ce programme compare deux longues séquences de caractères contenues dans des fichiers **FASTA**.  
Il détecte les substitutions entre les deux séquences et identifie les trois plus longues régions maximales sans substitutions.

Le programme est disponible en deux versions :

- une version en **Python** ;
- une version en **C++**.

Les séquences sont traitées par blocs grâce au paramètre `buffer_size`, afin d’éviter de charger toute la séquence en mémoire.

## Fonctionnalités

- Lecture de fichiers FASTA
- Comparaison de deux séquences caractère par caractère
- Détection des substitutions
- Génération d’un fichier CSV contenant les substitutions
- Détection des régions maximales sans substitutions
- Conservation des trois plus longues régions
- Calcul de statistiques pour chaque région
- Implémentation en Python et en C++
- Étude de l’impact de `buffer_size` sur le temps d’exécution

## Structure du projet

```txt
.
├── compare_strings.py
├── compare_strings.cpp
├── Region.py
├── TopRegions.py
├── Region.hpp
├── Region.cpp
├── TopRegions.hpp
├── TopRegions.cpp
├── Makefile
├── rapport.pdf
└── README.md
```

## Utilisation

### Version Python

```bash
python compare_strings.py chemin_ref chemin_seq chemin_output_subs chemin_output_regions buffer_size
```

Exemple :

```bash
python compare_strings.py data/ref.fasta data/seq.fasta output/substitutions.csv output/regions.csv 1000
```

### Version C++

Compilation :

```bash
make
```

Exécution :

```bash
./compare_strings chemin_ref chemin_seq chemin_output_subs chemin_output_regions buffer_size
```

Exemple :

```bash
./compare_strings data/ref.fasta data/seq.fasta output/substitutions.csv output/regions.csv 1000
```

## Arguments

| Argument | Description |
|---|---|
| `chemin_ref` | Chemin vers le fichier FASTA contenant la séquence de référence |
| `chemin_seq` | Chemin vers le fichier FASTA contenant la séquence à comparer |
| `chemin_output_subs` | Chemin vers le fichier CSV des substitutions |
| `chemin_output_regions` | Chemin vers le fichier CSV des régions sans substitutions |
| `buffer_size` | Nombre de caractères lus à chaque itération |

## Format du fichier des substitutions

Le fichier CSV des substitutions ne contient pas d’en-tête.

Format :

```csv
nom_sequence,position,caractere_reference,caractere_sequence
```

Exemple :

```csv
chr1,2,C,T
chr1,6,T,A
chr1,10,G,T
```

## Format du fichier des régions

Le fichier CSV des régions ne contient pas d’en-tête et contient au maximum trois lignes.

Format :

```csv
nom_sequence,debut,fin,longueur,fraction_longueur,fraction_A,fraction_C,fraction_G,fraction_T,fraction_N
```

Exemple :

```csv
chr1,3,5,3,0.23,0.00,0.33,0.67,0.00,0.00
chr1,7,9,3,0.23,0.67,0.00,0.00,0.33,0.00
chr1,0,1,2,0.15,1.00,0.00,0.00,0.00,0.00
```

## Exemple

Fichier `ref.fasta` :

```txt
>chr1
AACCGGTTAAGCC
```

Fichier `seq.fasta` :

```txt
>chr1
AATCGGATAATCC
```

Sortie attendue pour les substitutions :

```csv
chr1,2,C,T
chr1,6,T,A
chr1,10,G,T
```

Sortie attendue pour les régions :

```csv
chr1,3,5,3,0.23,0.00,0.33,0.67,0.00,0.00
chr1,7,9,3,0.23,0.67,0.00,0.00,0.33,0.00
chr1,0,1,2,0.15,1.00,0.00,0.00,0.00,0.00
```

## Compilation C++

Le programme C++ peut être compilé avec les options suivantes :

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all -O2 *.cpp -o compare_strings
```

Ou avec le Makefile :

```bash
make
```

## Étude expérimentale

Le projet inclut une étude expérimentale de l’influence de `buffer_size` sur le temps d’exécution.

Les valeurs testées sont :

```txt
1, 10, 100, 1000, 10000, 100000, 1000000
```

Les résultats sont présentés dans un graphique en échelle log-log afin de comparer les performances des versions Python et C++.

## Gestion mémoire

Le programme traite les fichiers par blocs pour limiter l’utilisation mémoire.

En C++, les buffers sont alloués dynamiquement selon la valeur de `buffer_size`, puis libérés correctement après utilisation.

## Licence

Ce projet est publié à des fins académiques.
