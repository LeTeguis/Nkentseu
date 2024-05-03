#!/usr/bin/env python3

import configuration as conf
from pathlib import Path


def GetParentFolder(index=0):
    file = Path(__file__).resolve()
    return file.parents[index]


# solution name
SOLUTION_NAME = "Nkentseu"

# library type ["STATIC_LIB", "DYNAMIC_LIB"]
LIB_TYPE = "DYNAMIC_LIB"

# graphics api type ["NKENTSEU_GRAPHICS_API_SOFTWARE", "NKENTSEU_GRAPHICS_API_OPENGL", "NKENTSEU_GRAPHICS_API_VULKAN", "NKENTSEU_GRAPHICS_API_DIRECTX11", "NKENTSEU_GRAPHICS_API_DIRECTX12", "NKENTSEU_GRAPHICS_API_METAL"]
GRAPHICS_API = "NKENTSEU_GRAPHICS_API_OPENGL"

# project name
PROJECTS_NAME = {
    "." : "",
    "Stb" : "External/Libs/Stb",
    "Glad" : "External/Libs/Glad",
    "NSystem" : "Core/NSystem",
    "Logger" : "Core/Logger",
    "Ntsm" : "Core/Ntsm",
    "Unitest" : "Core/Unitest",
    "Nkentseu" : "Core/Nkentseu",
    "Unkeny" : "Engine/Unkeny",
    "GlobalTests" : "Tests/GlobalTests",
    "Window" : "Exemples/Window",
    "UnkenyEditor" : "Engine/UnkenyEditor"
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