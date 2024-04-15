#!/usr/bin/env python3

from platform import system
import os
from enum import Enum


# Platform detection

class Platforme(Enum):
    WINDOWS = "windows"
    LINUX = "linux"
    MACOS = "macos"
    UNKNOW = ""


def GetPlatform():
    """
    Détermine la plateforme du système actuel.

    Retourne :
        Platforme : L'identifiant de la plateforme (Platforme.WINDOWS, Platforme.LINUX, Platforme.MACOS, ou Platforme.UNKNOW si non supportée).
    """

    platform_name = system().lower()
    if platform_name == "windows":
        return Platforme.WINDOWS
    elif platform_name == "linux":
        return Platforme.LINUX
    elif platform_name == "darwin":
        return Platforme.MACOS
    else:
        Platforme.UNKNOW = platform_name
        return Platforme.UNKNOW


def IsWindows():
    """
    Vérifie si le système actuel est Windows.

    Retourne :
        bool : True si Windows, False sinon.
    """

    return GetPlatform() == Platforme.WINDOWS


def IsLinux():
    """
    Vérifie si le système actuel est Linux.

    Retourne :
        bool : True si Linux, False sinon.
    """

    return GetPlatform() == Platforme.LINUX


def IsMacos():
    """
    Vérifie si le système actuel est macOS.

    Retourne :
        bool : True si macOS, False sinon.
    """

    return GetPlatform() == Platforme.MACOS


# Ide Detection

class ResultVariableStatus(Enum):
    CORRUPTED = 1  # Chemin différent
    EXISTS_CORRECT = 2  # Existe et chemin correct
    DOES_NOT_EXIST = 3  # N'existe pas


def CheckVariableStatus(nom_variable, chemin_variable):
    """
    Vérifie l'état d'une variable d'environnement.

    Args:
        nom_variable (str): Le nom de la variable d'environnement.
        chemin_variable (str): Le chemin à vérifier.

    Returns:
        ResultVariableStatus: L'énumération indiquant l'état de la variable.
    """

    valeur_existante = os.getenv(nom_variable)
    if valeur_existante is None:
        return ResultVariableStatus.DOES_NOT_EXIST
    elif valeur_existante == chemin_variable:
        return ResultVariableStatus.EXISTS_CORRECT
    else:
        return ResultVariableStatus.CORRUPTED


def AddVariable(nom_variable, chemin_variable):
    """
    Ajoute ou met à jour une variable d'environnement en fonction de son état.

    Args:
        nom_variable (str): Le nom de la variable d'environnement.
        chemin_variable (str): Le chemin à ajouter ou mettre à jour.
    """

    result = CheckVariableStatus(nom_variable, chemin_variable)
    print(nom_variable)
    print(chemin_variable)

    if result == ResultVariableStatus.DOES_NOT_EXIST:
        print("Variable n'existe pas. Ajout en cours...")
        os.environ[nom_variable] = chemin_variable
        return True
    elif result == ResultVariableStatus.CORRUPTED:
        confirm_change = input("Variable existe avec un chemin différent. Modifier? (O/N) ")
        if confirm_change.lower() in ["o", "oui"]:
            os.environ[nom_variable] = chemin_variable
            print("Variable mise à jour avec le nouveau chemin.")
            return True
        else:
            print("Variable conservée avec le chemin existant.")
            return False
    else:
        print("Variable existe avec le chemin correct.")
        return True


VS_BUILD_NAME = "VS_BUILD_PATH"
VS_PATH="\"C:/Programmes/Microsoft Visual Studio/2022/Professional/Common7/IDE/devenv.exe\""
VS_BUILD_PATH="C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe"
VS_VERSION_INSTALLER = "2022"


# executable

def GetExecutable(exe):
    """
    Fonction pour obtenir le nom du fichier exécutable en fonction de la plateforme.

    Args:
        exe (str): Le nom de base du fichier exécutable.

    Returns:
        str: Le nom complet du fichier exécutable avec l'extension appropriée pour la plateforme.
    """
    
    platform_name = GetPlatform()

    if platform_name == Platforme.LINUX:
        # Linux utilise généralement des noms de fichiers sans extension
        return exe
    elif platform_name == Platforme.MACOS:
        # macOS utilise des noms de fichiers avec l'extension .app
        return f'{exe}.app'
    elif platform_name == Platforme.WINDOWS:
        # Windows et autres plateformes utilisent généralement des noms de fichiers avec l'extension .exe
        return f'{exe}.exe'
    raise ValueError(f"Plateforme '{Platforme.UNKNOW}' non supportée")


