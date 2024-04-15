import os
import sys
import importlib
from pathlib import Path


def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


import configuration as conf


commands_dir = "./Scripts/commands"


def run_command(command_names=[]):
    """
    Affiche l'aide générale du shell nkentseu.
    """

    print("**Aide du shell nkentseu**")
    print("---------------------")

    if command_names == []:
        print("**Commandes disponibles :**")

        # Liste des commandes disponibles
        for file in os.listdir(commands_dir):
            if file.endswith(".py") and file != "__init__.py":
                command_name = file[:-3]
                print(f" - {command_name}")
    else:
        increment = 0
        for command_name in command_names:
            if increment == 0 and (command_name == "" or command_name == "?" or command_name == "help"):
                print("**Commandes disponibles :**")

                # Liste des commandes disponibles
                for file in os.listdir(commands_dir):
                    if file.endswith(".py")  and file != "__init__.py":
                        command_name = file[:-3]
                        print(f" - {command_name}")
                increment += 1
            
            else:
                help_command(command_name)

    print("\n\n**Utilisation :**")
    print(" - `commande [arguments]`")
    print(" - `commande [arguments] ; commande [arguments]`")
    print("**Aide pour une commande spécifique :**")
    print(" - `help commande`")


def help_command(command_name):
    """
    Affiche l'aide d'une commande spécifique.

    Args:
        command_name (str): Nom de la commande.
    """
    command_module = ""
    # Importation du module de la commande
    try:
        command_module = importlib.import_module(command_name)
    except ModuleNotFoundError:
        print(f"Commande inconnue : '{command_name}'")
        return

    # Vérification de la présence de la fonction help_command
    if not hasattr(command_module, "help_command"):
        print(f"La commande '{command_name}' n'a pas d'aide disponible.")
        return

    # Exécution de la fonction `help_command` du module
    command_module.help_command()


if __name__ == '__main__': 
    run_command(sys.argv[1:])

    sys.exit(0)