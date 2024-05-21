# NKENTSEU

La suite Nkentseu est un ensemble d'API et de logiciels graphiques multiplateformes qui comprendra plus tard :

- Moteur de jeu ;
- Application d'animation 2D/3D ;
- Application de simulation ;
- Application de modélisation ;
- Application de sculpture numérique ;
- Etc.

À l'état actuel, ce dépôt contient le noyau de la suite appelée Nkentseu et le moteur de jeu en cours de développement Unkeny. Ce dépôt est laissé gratuitement et donne la possibilité à toute personne souhaitant contribuer à son développement de nous contacter pour aider à sa construction.

## Utilisation du référentiel

Le référentiel actuel est structuré de manière très simple et contient des éléments de base à activer pour pouvoir compiler et exécuter l'ensemble des projets.

### Hiérarchie de la solution

#### La solution

La solution est structurée de la manière suivante où l'on retrouve dans le dossier racine plusieurs projets aussi importants les uns que les autres :

- [Core](./Core): le dossier core contient les API du noyau du système, notamment les APIs :
    - [NSystem](./Core/NSystem) : qui contient les déclarations des types de base, la gestion de la mémoire et bien d'autres.
    - [Logger](./Core/Logger) : qui contient notre système de journalisation.
    - [Ntsm](./Core/Ntsm) : qui contient notre bibliothèque de maths.
    - [Unitest](./Core/Unitest) : qui est notre bibliothèque de test unitaire.
    - [Nkentseu](./Core/Nkentseu) : qui est notre API graphique similaire à la SDL2 ou à la SFML, etc.
- [Engine](./Engine) : engine contient notre API de moteur de jeu qui descend directement de Nkentseu et l'éditeur du moteur pour l'instant.
- [Exemples](./Exemples) : va contenir les applications d'exemple développées soit avec Nkentseu soit avec le moteur ou avec toute autre partie de notre système.
- [Tests](./Tests) : contient l'ensemble de nos scripts qui utilisent notre API de test unitaire pour tester certains systèmes conçus.
- [External](./External) : va contenir notre générateur premake et toutes les parties tierces que nous aurons utilisées dans nos systèmes.

## Configuration

La configuration actuelle du projet est gérée par des scripts `nken`.

### Configuration Windows
- Pour configurer sous Windows, vous devez disposer de Visual Studio Code ou de tout autre IDE pris en charge, tel que Visual Studio (la compilation du projet sur Windows se fait via MSVC).
- Étant donné que nous utilisons MSVC sous Windows, vous devez modifier les constantes [`VS_PATH`](./Scripts/configuration.py), [`VS_BUILD_PATH`](./Scripts/configuration.py) et [`VS_VERSION_INSTALLER`](./Scripts/configuration.py) dans le fichier [`./Scripts/configuration.py`](./Scripts/configuration.py) en fonction de votre version de Visual Studio.
- Comme toutes les commandes sont exécutées sous Windows à partir de [`./nken.bat`](./nken.bat), si vous souhaitez utiliser uniquement `nken`, vous devez définir le chemin vers le projet actuel dans les variables d'environnement système.

### Configuration Linux
- Si vous utilisez WSL sous Windows, vous devez :
    * Ouvrir un terminal et saisir `nano ~/.bashrc`, puis ajouter les lignes suivantes au code :
        ```markdown
        export DISPLAY=localhost:0.0\
        export LIBGL_ALWAYS_INDIRECT=0\
        export MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA  # Remplacer "NVIDIA" par le nom de votre GPU\
        ```
    * Ensuite, quitter nano en sauvegardant les modifications et saisir la commande `source ~/.bashrc` pour appliquer les changements.
    * Assurez-vous de remplacer `"NVIDIA"` dans la variable `MESA_D3D12_DEFAULT_ADAPTER_NAME` par le nom correct de votre GPU. Vous pouvez trouver le nom de votre GPU dans les paramètres de votre système d'exploitation ou en utilisant l'outil `glxinfo`.
    * La version d'`OpenGL` prise en charge par WSL2 est actuellement `4.2`, mais sur Ubuntu et Windows, vous pouvez atteindre la version `4.6`, en fonction de votre carte graphique.

    
- Comme toutes les commandes sont exécutées sous Linux à partir de [`./nken.sh`](./nken.sh), si vous souhaitez utiliser uniquement `nken`, vous devez saisir `nano ~/.bash_aliases`, ajouter l'alias `alias nken="./nken.sh"` (en remplaçant `./nken.sh` par le chemin complet si nécessaire, bien que cela soit déconseillé), puis saisir `source ~/.bash_aliases` après avoir quitté nano depuis un terminal.

## Utilisation du code
Selon que vous utilisez `nken`, [`./nken.bat`](./nken.bat) ou [`./nken.sh`](./nken.sh) (que je vais renommer pour simplifier par `nken`), lorsque vous êtes dans le répertoire racine du projet, vous pouvez exécuter les commandes suivantes :

- `[nken init]` : lorsque vous estimez que le projet n'a jamais été initialisé pour installer certains outils de base sur Windows, Linux ou macOS. Cette commande ne doit être exécutée qu'une seule fois par machine, ou plusieurs fois si vous estimez avoir désinstallé certains outils par erreur.
- `[nken gen]` : pour générer les projets en fonction de la plateforme cible.
- `[nken clean]` : pour vider ou supprimer les dossiers de build et de génération. Vous devez ensuite exécuter à nouveau `nken gen` pour continuer à travailler sur les projets.
- `[nken build]` : pour compiler le projet en fonction de la plateforme cible.
- `[nken run]` ou `[nken run proj1 proj2 ... projn]` : pour exécuter le projet par défaut défini dans le fichier [`./Script/manager.py`](./Scripts/manager.py), ou pour exécuter l'un des projets exécutables situés dans le même fichier.
- `[nken create]` : la commande `create` permet de crée un fichier `(.cpp et .h)`, une `class`, une `structure (struct)`, et une `enumeration (enum)` en suivant certains principe (lire la documentation pour en savoir plus). vous pouvez decider de modifier la présentation de ces fichier en modifant directement le code source de la command ou si vous voulez utiliser comme c'est actuellement, il vous faut crée un fichier dans `profil.py` dans le dossier `Scripts/user` e y ajouter les informations suivants:
    ```python
    PROFIL = {
        "family_name": "votre nom",
        "first_name": "votre prenom",
        "description": "une description", # facultatif
        "function": "votre fonction" # facultatif
    }
    ```
    et n'oublier pas de définir le nom de la structure si elle n'est as déjà faite.

Si vous souhaitez ajouter un autre projet, n'oubliez pas de le spécifier dans [`./Scripts/manager.py`](./Scripts/manager.py), dans [`./premake5.lua`](./premake5.lua), dans [`./Scripts/config.lua`](./Scripts/config.lua), et de créer le fichier `premake5.lua` correspondant pour le projet en question.

Copyright (c) 2024 Rihen. All rights reserved.