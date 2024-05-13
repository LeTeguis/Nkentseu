import datetime
import os
import sys
from pathlib import Path
import subprocess


# Ajoute le dossier parent au chemin de recherche des modules Python
def AddParentModule():
    file = Path(__file__).resolve()
    parent, root = file.parent, file.parents[1]
    sys.path.append(str(root))


AddParentModule()


# Importe le module de configuration
import configuration as conf
import manager as man

import user.teuguia_tadjuidje_rodolf_sederis as ttrs
import user.nguengoue_louise_morena as nlm

def help_command():
    """Affiche l'aide de la commande `create`."""

    # Détermine l'extension du fichier de création en fonction de la plateforme
    extension_file = "bat" if conf.GetPlatform() == conf.Platforme.WINDOWS else "sh"

    # Affiche l'aide
    print(f"**./nken.{extension_file} create**")
    print("Génère un fichier source et un fichier d'en-tête en fonction des paramètres spécifiés.")
    print("Utilisation:")
    print("   create -project_name <nom_projet> -file_path <chemin_fichier> -generate_type <type_generation> -file_name <nom_fichier> [options]")
    print("Options:")
    print("   -user_who_created <nom_utilisateur>: Spécifie le nom de l'utilisateur qui crée le fichier.")
    print("   -company_who_created <nom_entreprise>: Spécifie le nom de l'entreprise qui crée le fichier.")
    print("   -namespace_name <nom_namespace>: Spécifie le nom du namespace pour le fichier généré.")
    print("   -api_definition <definition_api>: Spécifie la définition de l'API pour le fichier généré.")
    print("   -has_pch <True/False>: Indique si le fichier généré utilise un fichier d'en-tête précompilé (PCH).")
    print("   -constructor <True/False>: Indique si le fichier généré inclut un constructeur par défaut.")

def firstLower(strName):
    return strName[0].lower() + strName[1:]

def classGeneratorHeader(apiDefinition, className):
    return f'''
    class {apiDefinition} {className} {{
        public:
            {className}();
            ~{className}();

            std::string ToString();
            friend std::string ToString(const {className}& {firstLower(className)});
        private:
        protected:
    }};
'''

def structHeaderGenerator(apiDefinition, structName, hasConstructor):
    constructor = ""
    if hasConstructor:
        constructor = f'''
{structName}();
~{structName}();
'''
    return f'''
    struct {apiDefinition} {structName} {{
        {constructor}
        std::string ToString();
        friend std::string ToString(const {structName}& {firstLower(structName)});
    }};
'''

def generateEnumHeader(apiDefinition, enumName, typeSpecifier, typeData):
    enumType = "enum"
    if typeSpecifier == "in_struct":
        enumType = "struct"
    elif typeSpecifier == "in_class":
        enumType = "class"
    elif typeSpecifier == "class":
        enumType = "enum class"
    
    accessSpecifier = "public:" if enumType == "class" else ""
    content = "NotDefine"
    if enumType == "class" or enumType == "struct":
        content = f'''
        using Code = {typeData};
        enum : Code {{
            NotDefine
        }};
'''
    return f'''
{enumType} {apiDefinition} {enumName} {{
    {accessSpecifier}
        {content}
}};
'''

def enumHeaderGenerator(apiDefinition, enumName, subType, typeData):
    if subType in ["enum", "class", "in_class", "in_struct"]:
        return generateEnumHeader(apiDefinition, enumName, subType, typeData)
    return ""

def classGeneratorCpp(className, hasConstructor):
    constructor = ""
    if hasConstructor:
        constructor = f'''
    // Constructor
    {className}::{className}() {{
        // Ajoutez votre code de constructeur ici
    }}

    // Destructor
    {className}::~{className}() {{
        // Ajoutez votre code de destructeur ici
    }}
'''
    return f'''{constructor}
    std::string {className}::ToString() {{
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }}

    std::string ToString(const {className}& {firstLower(className)}) {{
        return {firstLower(className)}.ToString();
    }}
'''

def fileSeparater(filename):
    separated = ''
    for char in filename:
        if char.isupper():
            separated += '_' + char
        else:
            separated += char
    return separated.lstrip('_')

def formatFileDescription(fileDescription):
    lines = fileDescription.split('\n')  # Sépare le texte en lignes
    formatted_text = []
    
    for line in lines:
        while len(line) > 45:
            # Si la longueur de la ligne dépasse 45 caractères, la découpe et ajoute * devant chaque morceau
            formatted_text.append('* ' + line[:45])
            line = line[45:]
        if line.strip():  # Vérifie si la ligne n'est pas vide après avoir retiré les espaces
            formatted_text.append('* ' + line)
    
    return '\n'.join(formatted_text)


def headerGenerated(fileName, content, userWhoCreated, companyWhoCreated, fileDescription):
    guard = fileSeparater(fileName).upper()
    creationDate = datetime.date.today().strftime("%Y-%m-%d")
    creationTime = datetime.datetime.now().strftime("%I:%M:%S %p")
    creationYear = datetime.date.today().year
    format_file_description = "" if fileDescription.strip() == "" else f'''
/*
{formatFileDescription(fileDescription)}
*/
'''
    return f'''//
// Created by {userWhoCreated} on {creationDate} at {creationTime} AM.
// Copyright (c) {creationYear} {companyWhoCreated}. All rights reserved.
//
{format_file_description}
#ifndef __{guard}_H__
#define __{guard}_H__

#pragma once

{content}

#endif  // __{guard}_H__!'''

def cppGenerator(fileName, content, hasPch, projectName, userWhoCreated, companyWhoCreated):
    guard = fileSeparater(fileName).upper()
    creationDate = datetime.date.today().strftime("%Y-%m-%d")
    creationTime = datetime.datetime.now().strftime("%I:%M:%S %p")
    creationYear = datetime.date.today().year

    pchInclude = ""
    if hasPch:
        pchInclude = f'#include "{projectName}Pch/ntspch.h"'

    return f'''//
// Created by {userWhoCreated} on {creationDate} at {creationTime}.
// Copyright (c) {creationYear} {companyWhoCreated}. All rights reserved.
//

{pchInclude}
#include "{fileName}.h"

{content}'''

def separateNamespace(namespaceName):
    return namespaceName.split("::")

def namespaceGuard(namespaceName, content):
    namespaces = separateNamespace(namespaceName)

    # Filtrer les namespaces invalides
    filtered_namespaces = [ns for ns in namespaces if ns and ns[0].isalpha() and all(c.isalnum() or c == '_' for c in ns)]

    if not filtered_namespaces:
        return content

    lastNamespaceIndex = len(filtered_namespaces) - 1
    namespaceIndent = ""

    while lastNamespaceIndex >= 0:
        if lastNamespaceIndex == len(filtered_namespaces) - 1:
            namespaceIndent = f'''namespace {filtered_namespaces[lastNamespaceIndex]} {{
    {content}
}}  //  {filtered_namespaces[lastNamespaceIndex]}'''
        else:
            namespaceIndent = f'''namespace {filtered_namespaces[lastNamespaceIndex]} {{
    {namespaceIndent}
}}  //  {filtered_namespaces[lastNamespaceIndex]}'''
        lastNamespaceIndex -= 1

    return namespaceIndent

def generateFile(generatorProperties):
    headerFile = ""
    cppFile = ""
    userWhoCreated = ""
    companyWhoCreated = ""
    namespaceName = ""
    apiDefinition = ""
    hasPch = False

    if "project_name" not in generatorProperties or "file_path" not in generatorProperties \
            or "generate_type" not in generatorProperties or "file_name" not in generatorProperties:
        return False

    projectName = generatorProperties["project_name"]
    filePath = generatorProperties["file_path"]
    generateType = generatorProperties["generate_type"]
    fileName = generatorProperties["file_name"]

    if "user_who_created" in generatorProperties:
        userWhoCreated = generatorProperties["user_who_created"]

    if "company_who_created" in generatorProperties:
        companyWhoCreated = generatorProperties["company_who_created"]

    if "namespace_name" in generatorProperties:
        namespaceName = generatorProperties["namespace_name"]

    if "api_definition" in generatorProperties:
        apiDefinition = generatorProperties["api_definition"]

    if "has_pch" in generatorProperties:
        hasPch = generatorProperties["has_pch"]
        
    contentHeader = ""
    contentCpp = ""

    if generateType == "enum":
        subtype = "enum"
        if "sub_type" in generatorProperties:
            subtype = generatorProperties["sub_type"]
        subTypeList = ["enum", "in_class", "in_struct", "class"]
        if subtype not in subTypeList:
            return False
            
        enumName = fileName

        typeData = generatorProperties.get("type_data", "")

        contentHeader = enumHeaderGenerator(apiDefinition, enumName, subtype, typeData)
        
        if not contentHeader:
            return False
    elif generateType == "struct":
        structName = fileName
        hasConstructor = generatorProperties.get("constructor", False)
        contentHeader = structHeaderGenerator(apiDefinition, structName, hasConstructor)
        contentCpp = classGeneratorCpp(structName, hasConstructor)
    elif generateType == "class":
        className = fileName
        contentHeader = classGeneratorHeader(apiDefinition, className)
        contentCpp = classGeneratorCpp(className, True)  # Fixed here
    
    if not contentHeader:
        return False
    
    headerFile = headerGenerated(fileName, namespaceGuard(namespaceName, contentHeader), userWhoCreated, companyWhoCreated, "")

    if contentCpp:
        cppFile = cppGenerator(fileName, namespaceGuard(namespaceName, contentCpp), hasPch, projectName, userWhoCreated, companyWhoCreated)

    if not headerFile:
        return False

    # Écrire le contenu dans les fichiers correspondants
    file_path = os.path.join(filePath, fileName)
    with open(f'{file_path}.h', 'w') as header_file:
        header_file.write(headerFile)

    if cppFile:
        with open(f'{file_path}.cpp', 'w') as cpp_file:
            cpp_file.write(cppFile)

    return True

def parse_arguments(args):
    current_user = man.USER_DATA[man.CURRENT_USER]
    user_name = current_user["family_name"]
    user_first_name = current_user["first_name"]

    # Initialiser les propriétés du générateur avec des valeurs par défaut
    generatorProperties = {
        "project_name": "",
        "file_path": "",
        "generate_type": "",
        "file_name": "",
        "user_who_created": f"{user_name} {user_first_name}",
        "company_who_created": man.COMPANY_NAME,
        "copyright_date": man.COMPANY_COPYRIGHT,
        "namespace_name": man.NAMESPACE_PROJECT,
        "api_definition": man.API_DEFINITION,
        "has_pch": True,
        "constructor": True,
    }

    # Parcourir les arguments en ligne de commande et mettre à jour les propriétés du générateur
    i = 0  # Commencer à partir de l'indice 0 pour ignorer le nom du script
    while i < len(args) - 1:
        if args[i].startswith("-"):
            #prop_name = args[i][1:].replace("_", " ").title().replace(" ", "")  # Convertir en CamelCase
            prop_name = args[i][1:].strip()  # Convertir en CamelCase
            prop_value = args[i + 1]
            generatorProperties[prop_name] = prop_value
            #print(f"property = {prop_name}, value = {prop_value}")
            i += 2
        else:
            print(f"Argument invalide: {args[i]}")
            return None
    
    # Vérifier si toutes les propriétés nécessaires sont définies
    required_properties = ["project_name", "file_path", "generate_type", "file_name"]
    for prop in required_properties:
        if generatorProperties[prop] == "":
            print(f"La propriété '{prop}' est manquante.")
            return None
    
    return generatorProperties

def main():
    # Récupérer les arguments en ligne de commande
    args = sys.argv[1:]

    # Vérifier si des arguments ont été fournis
    if not args:
        print("Veuillez fournir des arguments en ligne de commande.")
        return 1

    # Analyser les arguments et récupérer les propriétés du générateur
    generatorProperties = parse_arguments(args)
    if generatorProperties is None:
        return 2

    # Vérifier si le nom du projet existe dans PROJECTS_NAME
    project_name = generatorProperties["project_name"]
    if project_name not in man.PROJECTS_NAME:
        print(f"Le nom du projet '{project_name}' n'existe pas dans PROJECTS_NAME.")
        return 3

    # Concaténer le chemin du projet avec le nom du projet pour obtenir le chemin complet du fichier
    file_path = man.PROJECTS_NAME[generatorProperties["project_name"]]
    file_path = file_path.rstrip('/')  # Supprimer le slash final si présent
    #print(generatorProperties['file_name'])
    generatorProperties["file_path"] = os.path.join(file_path, "src", generatorProperties['file_path'])

    # Générer le fichier en fonction des propriétés définies
    if generateFile(generatorProperties):
        print("Le fichier a été généré avec succès.")
        return 0
    print("Une erreur s'est produite lors de la génération du fichier.")
    return 4

if __name__ == "__main__":
    exit_code = main()

    sys.exit(exit_code)