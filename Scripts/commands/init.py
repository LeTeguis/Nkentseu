#!/usr/bin/env python3

import sys, os
from pathlib import Path
import subprocess
import winreg


def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf


def help_command():
    """Affiche l'aide de la commande `init`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} init**")
    print("Initialise les systèmes en téléchargeant les éléments nécessaires et en configurant le système.")



def AddVsPath():
    return conf.AddVariable(conf.VS_BUILD_NAME, conf.VS_BUILD_PATH)


def AddNkenVariable():
    file = Path(__file__).resolve()
    solution_path = file.parents[2]
    solution_path = f"{solution_path}"
    return conf.AddVariable(solution_path)


def InstallCompilateur():
    platform_name = conf.GetPlatform()

    if platform_name == conf.Platforme.LINUX:
        args_command = ["sudo", "apt", "install", "make"]
        result = subprocess.call(args_command)
        args_command = ["sudo", "apt", "install", "g++"]
        result = result if result != 0 else subprocess.call(args_command)
        return result
    elif platform_name == conf.Platforme.MACOS:
        return 0
    elif platform_name == conf.Platforme.WINDOWS:
        return 0
    raise ValueError(f"Plateforme '{conf.Platforme.UNKNOW}' non supportée")


def InitializeNken():
    AddNkenVariable()
    """script_name = "nken"
    script_path = os.path.join(os.getcwd(), script_name)
    
    # Vérifier si le script est déjà exécutable
    is_executable = os.access(script_path, os.X_OK)
    
    if not is_executable:
        # Déterminer le système d'exploitation
        if conf.GetPlatform() == conf.Platforme.WINDOWS:
            command = f"attrib +x {script_name}.bat"
        else:
            command = f"chmod +x {script_name}.sh"
        
        # Rendre le script exécutable
        os.system(command)
    """
    # Exécuter la commande avec nken help
    # os.system(f"nken help")


if __name__ == '__main__': 
    # exit_code = 0 if AddVsPath() == True else 1
    exit_code = InstallCompilateur()

    exit_code = InitializeNken() if exit_code == 0 else exit_code

    sys.exit(exit_code)