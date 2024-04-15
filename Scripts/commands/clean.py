#!/usr/bin/env python3

import os, sys
import shutil
from pathlib import Path


def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf


def help_command():
    """Affiche l'aide de la commande `clean`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} clean**")
    print("Nettoie le projet en supprimant les dossiers et fichiers de builds.")


# Fonction récursive pour supprimer les fichiers et dossiers correspondants
def clean(path, file_type, folders, depth=0, max_depth=2):
    # Vérifier la profondeur courante
    if depth >= max_depth:
        return
    
    # Supprimer les fichiers du type spécifié
    for f in os.listdir(path):
        file_path = os.path.join(path, f)
        for f in file_type:
            if os.path.isfile(file_path) and file_path.endswith(f):
                print("Deleted file : {}".format(file_path))
                os.remove(file_path)

    # Supprimer les dossiers spécifiés
    for d in os.listdir(path):
        dir_path = os.path.join(path, d)
        if os.path.isdir(dir_path) and d in folders:
            print("Deleted folder : {}".format(dir_path))
            shutil.rmtree(dir_path)
        elif os.path.isdir(dir_path):
            clean(dir_path, file_type, folders, depth=depth+1, max_depth=max_depth)



if __name__ == '__main__': 
    # Appeler la fonction pour le dossier courant et ses sous-répertoires jusqu'à une profondeur de 5
    file_type = [
        ".sln", ".csproj", ".vbproj", ".vcxproj", ".fsproj", ".sqlproj", ".vcxproj.user", "Makefile", "vcxproj.filters", ".xcodeproj",
        ".xcworkspace", ".pbxproj", ".xcconfig"
    ]

    folders = [".vs", "build", "build-int"]

    path = Path(".")
    clean(path, file_type, folders, max_depth=3)

    sys.exit(0)