#!/usr/bin/env python3

from platform import system, uname
import os, sys
from enum import Enum


# Platform detection

class Platforme(Enum):
    WINDOWS = "windows"
    LINUX = "linux"
    MACOS = "macos"
    UNKNOW = ""

USE_WSL = False
LINUX_WSL = 'NKENTSEU_LINUX_UNUSE_WSL'


def IsSubSystem():
    for x in uname():
        if "microsoft" in x.lower() or "windows" in x.lower():
            return True
    return False


def SubSystemSymbole():
    if IsSubSystem():
        return 'NKENTSEU_LINUX_USE_WSL'
    return 'NKENTSEU_LINUX_UNUSE_WSL'


def GetPlatform():
    """
    Détermine la plateforme du système actuel.

    Retourne :
        Platforme : L'identifiant de la plateforme (Platforme.WINDOWS, Platforme.LINUX, Platforme.MACOS, ou Platforme.UNKNOW si non supportée).
    """

    platform_name = system().lower()
    if platform_name == "windows":
        USE_WSL = False
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


def CheckVariableStatus(chemin_variable):
    path = os.environ.get('PATH', '')
    return chemin_variable in path.split(os.pathsep)


def AddVariable(chemin_variable):
    if CheckVariableStatus(chemin_variable):
        print(f"Le répertoire {chemin_variable} est déjà dans la variable PATH.")
        return 1

    os.environ['PATH'] += os.pathsep + chemin_variable

    # Mettre à jour la variable PATH pour la session en cours
    if IsWindows():
        os.system(f'setx PATH "%PATH%;" /S System')
    elif IsLinux():
        os.system(f'export PATH="$PATH:{chemin_variable}"')
    elif IsMacos():  # macOS
        os.system(f'export PATH="$PATH:{chemin_variable}"')

    print(f"Le répertoire {chemin_variable} a été ajouté à la variable PATH.")
    return 0



VS_PATH="\"C:/Programmes/Microsoft Visual Studio/2022/Community/Common7/IDE/devenv.exe\""
VS_BUILD_PATH="C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"
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


