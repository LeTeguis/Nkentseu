#!/usr/bin/env python3

import subprocess
import sys, os
from pathlib import Path


def AddParentModule():
    file = Path(__file__).resolve()
    root = file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf
import manager as man


def help_command():
    """Affiche l'aide de la commande `gen`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} gen**")
    print("Génère le groupe de projets en fonction de la plateforme définie dans le fichier ./Scripts/manager.py.")


def GetPremakeGenerator():
    """
    Détermine le générateur Premake en fonction de la plateforme.

    Retourne :
        str : Le nom du générateur Premake, une chaîne vide pour macOS 
                (car on utilise la commande premake5 directement), 
                ou une erreur si la plateforme n'est pas supportée.
    """

    platform_name = conf.GetPlatform()  # Assuming GetPlatform returns a Platforme enum value
    vs_version = conf.VS_VERSION_INSTALLER

    if platform_name == conf.Platforme.WINDOWS:
        #return f'gmake2'
        return f'vs{vs_version}'
    elif platform_name == conf.Platforme.LINUX:
        return 'gmake2'
    elif platform_name == conf.Platforme.MACOS:
        return 'xcode4'
    else:
        raise ValueError(f"Plateforme '{platform_name.value}' non supportée")


def GetPremakePath():
    """
    Détermine le chemin d'accès à l'exécutable Premake5 en fonction de la plateforme.

    Retourne :
        str : Le chemin complet vers l'exécutable Premake5.
    """

    premake_executable = ""
    platform_name = conf.GetPlatform()

    if platform_name == conf.Platforme.WINDOWS:
        premake_executable = '.exe'
    elif platform_name == conf.Platforme.LINUX:
        premake_executable = '.linux'
    elif platform_name == conf.Platforme.MACOS:
        premake_executable = ''
    else:
        raise ValueError(f"Plateforme '{platform_name.value}' non supportée")
    
    premake_path = os.path.join(man.GetParentFolder(1), "External", "Bin", "Premake5", f"premake5{premake_executable}")
    return premake_path


def GenerateProject():
    """
    Génère le projet en utilisant Premake5.

    Retourne :
        int : Le code de retour de l'appel à subprocess.call, indiquant la réussite ou l'échec.
    """

    argv_param = []  # Utiliser un tableau pour les arguments
    argv_param.append(f"--libtype={man.LIB_TYPE}")
    argv_param.append(f"--startupproject={man.DEFAULT_PROJECT}")
    argv_param.append(f"--graphicsapi={man.GRAPHICS_API}")
    argv_param.append(f"--linuxwinapi={man.LINUX_WIN_API}")

    # Ajoutez d'autres arguments à argv_param si nécessaire

    premake_path = GetPremakePath()
    generator = GetPremakeGenerator()
    platform_name = conf.GetPlatform()

    # Construisez la liste d'arguments pour subprocess.call
    call_args = [premake_path] + argv_param + [generator]

    # Utiliser platform-specific handling uniquement pour Windows (lancement cmd.exe)
    if platform_name == conf.Platforme.WINDOWS:
        call_args = ["cmd.exe", "/c"] + call_args  # Insérer "cmd.exe" au début pour Windows

    return subprocess.call(call_args)  # Appeler subprocess.call avec la liste d'arguments complète


if __name__ == '__main__': 
    # Run the project generation process
    exit_code = GenerateProject()
    sys.exit(exit_code)
