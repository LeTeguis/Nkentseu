#!/usr/bin/env python3

import os
import configuration as conf
from pathlib import Path


def GetParentFolder(index=0):
    file = Path(__file__).resolve()
    return file.parents[index]

# company name
COMPANY_NAME = "Rihen"
COMPANY_COPYRIGHT = "2024"

# solution name
SOLUTION_NAME = "Nkentseu"

# library type ["STATIC_LIB", "DYNAMIC_LIB"]
LIB_TYPE = "DYNAMIC_LIB"

# graphics api type ["NKENTSEU_GRAPHICS_API_SOFTWARE", "NKENTSEU_GRAPHICS_API_OPENGL", "NKENTSEU_GRAPHICS_API_VULKAN", "NKENTSEU_GRAPHICS_API_DIRECTX11", "NKENTSEU_GRAPHICS_API_DIRECTX12", "NKENTSEU_GRAPHICS_API_METAL"]
GRAPHICS_API = "NKENTSEU_GRAPHICS_API_VULKAN"

# linux window api type ["NKENTSEU_LINUX_WIN_API_XCB", "NKENTSEU_LINUX_WIN_API_XLIB"]
LINUX_WIN_API = "NKENTSEU_LINUX_WIN_API_XLIB"

# project name
PROJECTS_NAME = {
    "." : "",
    "Stb" : os.path.join("External", "Libs", "Stb"),
    "Glad" : os.path.join("External", "Libs", "Glad"),
    "NSystem" : os.path.join("Core", "NSystem"),
    "Logger" : os.path.join("Core", "Logger"),
    "Ntsm" : os.path.join("Core", "Ntsm"),
    "Unitest" : os.path.join("Core", "Unitest"),
    "Nkentseu" : os.path.join("Core", "Nkentseu"),
    "Unkeny" : os.path.join("Engine", "Unkeny"),
    "GlobalTests" : os.path.join("Tests", "GlobalTests"),
    "Window" : os.path.join("Exemples", "Window"),
    "UnkenyEditor" : os.path.join("Engine", "UnkenyEditor")
}

DEFAULT_PROJECT = "UnkenyEditor"

# configuration debug, release
CONFIG = "debug"

# architecture
ARCHITECTURE = "x86_64"

# Encoding run
ENCODING = "{}/build/bin/{}/{}/"

# build configuration
config = "{}{}".format(CONFIG[0].upper(), CONFIG[1:])
BUILD_CONFIG = "{}-{}-{}".format(config, conf.GetPlatform().value, ARCHITECTURE)

# path to run executable
EXE_PATH = {}
for name in PROJECTS_NAME:
    EXE_PATH[name] = ENCODING.format(GetParentFolder(1), BUILD_CONFIG, name)


# define c, cpp, h, hpp infos
NAMESPACE_PROJECT = "nkentseu"
API_DEFINITION = "NKENTSEU_API"


# define user is contribut

# teuguia tadjuidje rodolf sederis
TTRS = {
    "family_name": "TEUGUIA TADJUIDJE",
    "first_name": "Rodolf Séderis",
    "description": "CEO and co-founder of Rihen",
    "function": "Lead developer"
}

USER_DATA = {
    "ttrs" : TTRS
}

CURRENT_USER = "ttrs"