#!/usr/bin/env python3

import sys
from pathlib import Path
import subprocess


# Ajoute le dossier parent au chemin de recherche des modules Python
def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


# Importe le module de configuration
import configuration as conf


# Fonction pour afficher l'aide de la commande `init`
def help_command():
    """Affiche l'aide de la commande `init`."""

    # Détermine l'extension du fichier d'initialisation en fonction de la plateforme
    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    # Affiche l'aide
    print(f"**./nken.{extension_file} init**")
    print("Initialise les systèmes en téléchargeant les éléments nécessaires et en configurant le système.")


# Installe les compilateurs et les dépendances
def InstallCompilateur():
    platform_name = conf.GetPlatform()

    application_install = ["make", "g++", "nautilus -y", "gnome-text-editor -y"]
    lib_install = ["libx11-dev", "xorg-dev"]
    application_snap = ["--classic code"]

    if platform_name == conf.Platforme.LINUX:
        # Installe les applications nécessaires via apt-get
        for app in application_install:
            args_command = ["sudo", "apt", "install", app]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        
        # Installe les applications via Snap
        for app in application_snap:
            args_command = ["sudo", "snap", "install", app]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        
        # Installe les bibliothèques nécessaires via apt-get
        for lib in lib_install:
            args_command = ["sudo", "apt-get", "install", lib]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        return result
    elif platform_name == conf.Platforme.MACOS:
        # Pour MacOS, l'installation des outils est gérée différemment
        return 0
    elif platform_name == conf.Platforme.WINDOWS:
        # Pour Windows, l'installation des outils est gérée différemment
        return 0
    raise ValueError(f"Plateforme '{conf.Platforme.UNKNOW}' non supportée")


# Fonction principale pour initialiser Nken
def InitializeNken():
    exit_code = InstallCompilateur()
    return exit_code

if __name__ == '__main__': 
    exit_code = InitializeNken()

    sys.exit(exit_code)