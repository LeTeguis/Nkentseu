project "Glad"
    kind ("StaticLib")
    langageInformations()
    staticruntime "off"

    BuildsInfos("%{prj.name}")
    
    files {
        "src/**.c",
        "include/**.h",
    }

    includedirs {
        "./include",
        --"%{Internals.NSystem}/src",
    }

    filter "system:windows"
        removefiles {
            "src/glx.c",
            "include/glad/glx.h",
        }

    filter "system:macosx"
        removefiles {
            "src/wgl.c",
            "include/glad/wgl.h",
        }

    filter "system:linux"
        removefiles {
            "src/wgl.c",
            "include/glad/wgl.h",
        }

    filter {}

    defines {
		"_CRT_SECURE_NO_WARNINGS",
    }

    filter "system:windows"
        systemversion "latest"
		optimize "off"
        
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
        -- Ajout d'options de compilation pour toutes les configurations sous Linux
        buildoptions { "-fPIC", "-pthread" }

    filter "configurations:Debug"
        defines { "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH "}
        defines {  "NKENTSEU_DEBUG"}
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NKENTSEU_RELEASE" }
        runtime "Release"
        optimize "on"

    filter {}
