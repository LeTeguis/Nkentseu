project "Unkeny"
    kind (libraryType)
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    BuildsInfos("%{prj.name}")

    pchheader "%{prj.name}Pch/ntspch.h"
    pchsource "src/%{prj.name}Pch/ntspch.cpp"
    
    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.inl",
        "src/**.cpp"
    }

    includedirs {
        "./src/",
        "%{Internals.Nkentseu}/src",
        "%{Internals.Logger}/src",
        "%{Internals.NSystem}/src",
        "%{Internals.Ntsm}/src"
    }

    links {
        "NSystem",
        "Logger",
        "Ntsm",
        "Stb",
        "Nkentseu"
    }

    defines {
		"_CRT_SECURE_NO_WARNINGS", "GLFW_INCLUDE_NONE"
    }

    defineExport()
    defineApiInfo()
    -- defineGraphicApi()

    links {
    }

    filter "system:windows"
        systemversion "latest"
		optimize "off"

        links {
        }

        linkoptions { "-lpthread" }

    filter "system:macosx"
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["USeModernBuildSystem"] = "NO"
        }

        links {
        }

        -- Ajout d'options de compilation pour toutes les configurations sous macOS
        buildoptions { "-stdlib=libc++", "-fPIC", "-pthread" }

    filter "system:linux"
        links {
        }

        -- Ajout d'options de compilation pour toutes les configurations sous Linux
        buildoptions { "-fPIC", "-pthread" }

    filter "configurations:Debug"
        defines { "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH "}
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
