# NKENTSUU
La suite Nkentsuu est un ensemble de logiciel graphique multiplateforme qui sera plus tard constituer de :

    - Moteur de jeu ;
    - Application d'animation 2D/3D ;
    - Application de simulation ;
    - Application de modélisation ;
    - Application de sculpture numérique ;
    - Etc.
    
À l'état, se repository contient le noyau de suite nommé Nkentsuu et le moteur de jeu en cours de développement Noge.
Se référentiel est laisser gratuitement et donne la possibilité à toute personne voulant contribuer a son développement de nous contacter pour aider à sa construction.

## Utilisation du révérenciel
le référentiel actuelle est structurer d'une manière très simple et a des éléments de base à activer pour pouvoir compiler et exécuter l'ensembles des projets.

### Hiérarchie de la solution

#### La solution
La solution est structurer de la manière suivante:

    nkentsuu/
    │   ├── .gitignore
    ├── nkentsuu/
    │   ├── docs/
    │   ├── src/
    │   ├── premake5.lua
    │   ├── README.md
    │   └── ...
    ├── external/
    │   ├── bin/
    │   ├── libs/
    │   └── ...
    ├── tools/
    │   ├── run.py
    │   ├── gen.py
    │   ├── build.py
    │   └── ...
    ├── utest/
    │   ├── src/
    │   ├── premake5.lua
    │   └── README.md
    ├── nts.bat
    ├── nts.py
    ├── nts.sh
    ├── LICENCE
    ├── premake5.lua
    ├── README.md

* nkentsuu : c'est le répertoire racine de tous les projets de la suite nkentsuu
    -   nkentsuu : c'est le moteur graphique haute performance de base commun à toutes les applications de la suite
    -   external : c'est ou seront stoker les bibliothèques externes et les binaire utile pour le bon fonctionnement des projets (le third party)
    -   tools : les scripts de base pour installer les bibliothèques de manière portable
    -   utest : les tests unitaires et fonctionnelle pour chaque projet
    -   nts.bat, nts.py, nts.sh : represente les scripts de base pour acceder aux scripts de construction, de build, de run et bien d'autre

## Configuration
La configuration actuelle du projet passe par des scripts nts*

### Configuration Windows
- Pour configurer sur windows vous devez avoir visual studio community installer, visual studio code et wsl2
- Vous devez definir les variables d'environnement VS_BUILD_PATH et VS_PATH pour visual studio community, CODE_PATH pour visual studio code comme definit dans le fichier tools/tmps/script

### Pour toutes les autres plateforms windows compris
Allez dans le fichier tools/tmps/script puis ouvrir un terminal ubuntu ou wsl sur windows et comme indique
- Tapez nano ~/.bash_env
- Puis copie coller le code qui se trouve juste en bas avant le prochain //---
- Sortez de nano et tapez source ~/.bash_env
- Tapez  nano ~/.bash_aliases
- Puis copie et coller le code qui se trouve juste en bas avant le prochain //---
- Sortez de nano et tapez source ~/.bash_aliases
- Tapez nano ~/.bashrc
- Copie et coler le code suivant
    if [ -f ~/.bash_env ]; then
        . ~/.bash_env
    fi
- Sortez de nano et redemarez votre ordinateur pour configurer toute les commandes

## Utilisation du code
Pour utiliser le projet vous devez comprendre les script actuel

- Generation de projet : pour generer la solution vous devez entrer le code "nts gen"
- Build du projet : pour compiler le projet vous devez entrer le code "nts build"
- Run du projet : pour lancer le projet vous devez entrer le code "nts run"

si vous voulez ajouter un autre projet n'oubliez pas de le preciser dans ./tools/globals.py, dans ./premake5.lua, dans ./config.lua et de creer le fichier premake5.lua du projet en question
