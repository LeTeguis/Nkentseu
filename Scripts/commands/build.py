#!/usr/bin/env python3

import sys
import os
import glob
import subprocess
from pathlib import Path


def GetParentFolder(index=0):
    file = Path(__file__).resolve()
    return file.parents[index]


def AddParentModule():
    file = Path(__file__).resolve()
    root = file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf
import manager as man


def help_command():
    """Affiche l'aide de la commande `build`."""

    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    print(f"**./nken.{extension_file} build**")
    print("Compile tous les projets non mis à jour qui n'ont pas été compilés.")


def BuildWindows():
    """
    Runs the build for the Windows platform with proper quoting for spaces in paths and project names.

    Returns:
        int: The return code of the subprocess call, indicating success (0) or failure (> 0).
    """

    solution_path = os.path.join(GetParentFolder(2), f"{man.SOLUTION_NAME}.sln")

    # Vérifier l'existence de MSBuild
    if not os.path.exists(conf.VS_BUILD_PATH):
        print(f"**Erreur : MSBuild introuvable à l'emplacement spécifié : {conf.VS_BUILD_PATH}")
        return 1

    # Vérifier l'existence de la solution
    if not os.path.exists(solution_path):
        print(f"**Erreur : La solution introuvable à l'emplacement spécifié : {solution_path}")
        return 1

    # Construire la commande avec le chemin vérifié
    command = [conf.VS_BUILD_PATH, os.path.normpath(solution_path), f'/property:Configuration={man.CONFIG}']

    # Exécuter la commande de build
    try:
        return subprocess.call(command, shell=False)
    except OSError as e:
        print(f"Error during build: {e}")
        return 1  # Indicate failure with non-zero return code


def BuildLinux():
    """Builds projects on Linux considering dependencies."""

    result = 0  # Initialize result variable for success
    result = subprocess.run(("make", f'config={man.CONFIG.lower()}', 'all'))
    return result.returncode  # Return final result (0 for success)


def BuildMacos():
    """Builds projects on macOS considering dependencies (similar to BuildLinux)."""

    result = 0

    repertoir = os.getcwd()

    for project_name in man.PROJECTS_NAME:
        xcodeproj_file = os.path.join(man.PROJECTS_NAME[project_name], f"{project_name}.xcodeproj")

        if os.path.exists(xcodeproj_file):
            os.chdir(man.PROJECTS_NAME[project_name])

            result = subprocess.call(["xcodebuild", "-project", xcodeproj_file, "-configuration", man.CONFIG])

            os.chdir(repertoir)

            if result != 0:
                return result

    return result


def Build():
    """Builds the project based on the current platform."""

    platform_name = conf.GetPlatform()  # Assuming GetPlatform returns a Platforme enum value

    if platform_name == conf.Platforme.WINDOWS:
        return BuildWindows()
    elif platform_name == conf.Platforme.LINUX:
        return BuildLinux()
    elif platform_name == conf.Platforme.MACOS:
        return BuildMacos()
    else:
        raise ValueError(f"Platform '{platform_name.value}' not supported")


if __name__ == '__main__': 
    # Run the build process
    exit_code = Build()
    sys.exit(exit_code)
