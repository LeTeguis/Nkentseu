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
import manager as man


def help_command():
    """Affiche l'aide de la commande `clean`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} clean**")
    print("Nettoie le projet en supprimant les dossiers et fichiers de builds.")


# Fonction récursive pour supprimer les fichiers et dossiers correspondants
def clean_projects(file_type, build_folder):
    for project_name, project_path in man.PROJECTS_NAME.items():
        path = f"./{project_path}"
        for current_element in os.listdir(path):
            element_path = os.path.join(path, current_element)
            if os.path.isdir(element_path) and current_element in build_folder:
                print("============= [{}] (Deleted folder) | \"{}\"".format(project_name, element_path))
                shutil.rmtree(element_path)
            else:
                for current_file_type in file_type:
                    if os.path.isfile(element_path) and element_path.endswith(current_file_type):
                        print("============= [{}] (Deleted file) | \"{}\"".format(project_name, element_path))
                        os.remove(element_path)


if __name__ == '__main__': 
    # Appeler la fonction pour le dossier courant et ses sous-répertoires jusqu'à une profondeur de 5
    file_type = [
        ".sln", ".csproj", ".vbproj", ".vcxproj", ".fsproj", ".sqlproj", ".vcxproj.user", "Makefile", "vcxproj.filters", ".xcodeproj",
        ".xcworkspace", ".pbxproj", ".xcconfig"
    ]

    folders = [".vs", "build", "build-int"]

    clean_projects(file_type, folders)

    sys.exit(0)