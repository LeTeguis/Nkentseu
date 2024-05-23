#!/usr/bin/env python3

import os
import subprocess
import sys
import argparse
from pathlib import Path

def GetParentFolder(index=0):
    file = Path(__file__).resolve()
    return file.parents[index]

def AddParentModule():
    file = Path(__file__).resolve()
    root = file.parents[1]
    sys.path.append(str(root))

AddParentModule()

import configuration as conf
import manager as man

def compile_shader(shader_name, directory, shader_types_to_compile):
    platform_name = conf.GetPlatform()
    
    shader_types = {
        'vert': 'vertex',
        'frag': 'fragment',
        'comp': 'compute',
        'geom': 'geometry',
        'tesc': 'tesscontrol',
        'tese': 'tesseval'
    }

    for file in os.listdir(directory):
        if shader_name in file and not file.endswith(".spv"):
            file_path = os.path.join(directory, file)
            file_ext = file.split('.')[-2] if '.' in file else ''
            
            if file_ext in shader_types:
                if shader_types_to_compile and shader_types[file_ext] not in shader_types_to_compile:
                    continue

                shader_stage = shader_types[file_ext]
                output_file = f"{os.path.splitext(file_path)[0]}.spv" if file_path.endswith(".glsl") else f"{file_path}.spv"

                if platform_name == conf.Platforme.WINDOWS:
                    glslc = "glslc.exe"
                else:
                    glslc = "glslc"

                cmd = [glslc, f"-fshader-stage={shader_stage}", file_path, "-o", output_file]
                
                try:
                    subprocess.run(cmd, check=True)
                    print(f"Successfully compiled {file_path} to {output_file}")
                except subprocess.CalledProcessError as e:
                    print(f"Error compiling {file_path}: {e}")
            else:
                print(f"Skipping {file_path}: Unrecognized shader type")

    return 0

def remove_extension_if_matches(filename, extensions):
    while any(filename.endswith(ext) for ext in extensions):
        for ext in extensions:
            if filename.endswith(ext):
                filename = filename[: -len(ext)]
                break
    return filename

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compile GLSL shaders to SPIR-V using glslc.")
    parser.add_argument("--name", required=True, help="The name of the shader to search for.")
    parser.add_argument("--directory", default=man.DEFAULT_RESOURCE_DIRECTORY, help="The directory to search in (default is current directory).")
    parser.add_argument("--vert", action="store_true", help="Compile only vertex shaders.")
    parser.add_argument("--frag", action="store_true", help="Compile only fragment shaders.")
    parser.add_argument("--comp", action="store_true", help="Compile only compute shaders.")
    parser.add_argument("--geom", action="store_true", help="Compile only geometry shaders.")
    parser.add_argument("--tesc", action="store_true", help="Compile only tessellation control shaders.")
    parser.add_argument("--tese", action="store_true", help="Compile only tessellation evaluation shaders.")

    args = parser.parse_args()

    shader_types_to_compile = []
    if args.vert:
        shader_types_to_compile.append('vertex')
    if args.frag:
        shader_types_to_compile.append('fragment')
    if args.comp:
        shader_types_to_compile.append('compute')
    if args.geom:
        shader_types_to_compile.append('geometry')
    if args.tesc:
        shader_types_to_compile.append('tesscontrol')
    if args.tese:
        shader_types_to_compile.append('tesseval')

    # Remove extension if it matches any of the specified extensions
    name = remove_extension_if_matches(args.name, ['.glsl', '.vert', '.frag', '.comp', '.geom', '.tesc', '.tese'])

    exit_code = compile_shader(name, args.directory, shader_types_to_compile)

    sys.exit(exit_code)
