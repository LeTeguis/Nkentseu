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

# graphics api type ["GAPI_SOFTWARE", "GAPI_OPENGL", "GAPI_VULKAN", "GAPI_DIRECTX11", "GAPI_DIRECTX12", "GAPI_METAL"]
GRAPHICS_API = "GAPI_SOFTWARE"

# project name
PROJECTS_NAME = {
    "NSystem" : "Core/NSystem",
    "Logger" : "Core/Logger",
    "Ntsm" : "Core/Ntsm",
    "Unitest" : "Core/Unitest",
    "Nkentseu" : "Core/Nkentseu",
    "Unkeny" : "Engine/Unkeny",
    "GlobalTests" : "Tests/GlobalTests",
    "Window" : "Exemples/Windows",
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