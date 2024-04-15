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

    application_install = ["make", "g++", "nautilus -y", "gnome-text-editor -y"]
    lib_install = ["libx11-dev", "xorg-dev"]
    application_snap = ["--classic code"]

    if platform_name == conf.Platforme.LINUX:
        for app in application_install:
            args_command = ["sudo", "apt", "install", app]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        for app in application_snap:
            args_command = ["sudo", "snap", "install", app]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        for lib in lib_install:
            args_command = ["sudo", "apt-get", "install", lib]
            result = subprocess.call(args_command)
            if result != 0:
                return result
        return result
    elif platform_name == conf.Platforme.MACOS:
        return 0
    elif platform_name == conf.Platforme.WINDOWS:
        return 0
    raise ValueError(f"Plateforme '{conf.Platforme.UNKNOW}' non supportée")


def InitializeNken():
    AddNkenVariable()


if __name__ == '__main__': 
    # exit_code = 0 if AddVsPath() == True else 1
    exit_code = InstallCompilateur()

    sys.exit(exit_code)