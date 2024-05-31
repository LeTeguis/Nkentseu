import os
import re

def renommer_et_modifier_fichiers(repertoire, chaine_a_remplacer, nouvelle_chaine):
  for fichier in os.listdir(repertoire):
    nom_fichier, extension = os.path.splitext(fichier)
    nouveau_nom_fichier = nom_fichier.replace(chaine_a_remplacer, nouvelle_chaine) + extension

    # Renommer le fichier (sans ignorer la casse)
    if nom_fichier != nouveau_nom_fichier:
      os.rename(os.path.join(repertoire, fichier), os.path.join(repertoire, nouveau_nom_fichier))

    # Modifier le contenu du fichier (sans ignorer la casse)
    with open(os.path.join(repertoire, nouveau_nom_fichier), "r+") as fichier_ouvert:
      contenu_fichier = fichier_ouvert.read()
      contenu_fichier_modifie = contenu_fichier.replace(chaine_a_remplacer, nouvelle_chaine)
      if contenu_fichier != contenu_fichier_modifie:
        fichier_ouvert.seek(0)
        fichier_ouvert.write(contenu_fichier_modifie)
        fichier_ouvert.truncate()

# Exemple d'utilisation
repertoire = "D:\\Projets\\Rihen\\Logiciels\\Nkentseu\\2024\\Nkentseu\\Core\\Nkentseu\\src\\Nkentseu\\Graphics\\Internal\\Opengl"
chaine_a_remplacer = "Internal"
nouvelle_chaine = "Opengl"

renommer_et_modifier_fichiers(repertoire, chaine_a_remplacer, nouvelle_chaine)
