import os

def renommer_et_modifier_fichiers(repertoire, chaine_a_remplacer, nouvelle_chaine):
    # Parcourir tous les répertoires et sous-répertoires
    for racine, dossiers, fichiers in os.walk(repertoire):
        for fichier in fichiers:
            nom_fichier, extension = os.path.splitext(fichier)

            # Vérifier si le fichier est un .h ou .cpp
            if extension in ['.h', '.cpp']:
                chemin_complet_fichier = os.path.join(racine, fichier)

                # Modifier le contenu du fichier si nécessaire
                try:
                    with open(chemin_complet_fichier, "r+", encoding="latin-1") as fichier_ouvert:
                        contenu_fichier = fichier_ouvert.read()
                        contenu_fichier_modifie = contenu_fichier.replace(chaine_a_remplacer, nouvelle_chaine)
                        if contenu_fichier != contenu_fichier_modifie:
                            fichier_ouvert.seek(0)
                            fichier_ouvert.write(contenu_fichier_modifie)
                            fichier_ouvert.truncate()
                            print(f"Contenu modifié dans le fichier: {chemin_complet_fichier}")
                except Exception as e:
                    print(f"Erreur lors de la modification du fichier {chemin_complet_fichier}: {e}")

# Exemple d'utilisation
repertoire = "D:\\Projets\\Rihen\\Logiciels\\Nkentseu\\2024\\Nkentseu"
chaine_a_remplacer = "NTSSystem/"
nouvelle_chaine = "NTSCore/"

renommer_et_modifier_fichiers(repertoire, chaine_a_remplacer, nouvelle_chaine)

# Mettre en pause le script pour voir les résultats
input("Appuyez sur Entrée pour terminer...")

