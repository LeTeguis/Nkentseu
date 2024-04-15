#!/usr/bin/env python3

import sys, os
import subprocess
from pathlib import Path


def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf
import manager as man


def help_command():
    """Affiche l'aide de la commande `run`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} run [nom_du_projet] [nom_du_projet]**")
    print("Exécute chaque projet listé après `run` s'il s'agit d'une application exécutable présente dans la liste d'applications.")


def run_project(project_name):
    if not project_name:
        print("**ERREUR :** Veuillez spécifier un nom de projet.")
        return 1

    try:
        project_cwd = man.EXE_PATH[project_name]
    except KeyError:
        print(f"**ERREUR :** Le projet '{project_name}' n'existe pas.")
        return 1

    project_path = os.path.join(project_cwd, project_name)  # Recommandé pour la création de chemins
    
    try:
        return subprocess.run(project_path, check=True).returncode
    except FileNotFoundError as e:
        print(f"**ERREUR :** Le fichier '{project_name}' n'a pas été trouvé dans le répertoire '{project_cwd}'.")
        return 1
    except subprocess.CalledProcessError as e:
        print(f"**ERREUR :** L'exécution du projet '{project_name}' a échoué avec le code de retour {e.returncode}.")
        return 1


def run_default():
    return run_project(man.DEFAULT_PROJECT)


if __name__ == '__main__': 
    result = 0
    len_cmd = len(sys.argv)

    if len_cmd > 1:
        current_project_index = 1
        
        while current_project_index < len_cmd:
            result = run_project(sys.argv[current_project_index])

            if result != 0:
                sys.exit(result)
            current_project_index += 1
    else:
        result = run_default()


    sys.exit(result)
