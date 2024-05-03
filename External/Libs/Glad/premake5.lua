project "Glad"
    kind (libraryType)
    langageInformations()
    staticruntime "off"

    BuildsInfos("%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp",
        "include/**.h",
        "include/**.hpp",
        "include/**.c",
        "include/**.cpp",
    }

    includedirs {
        "./include",
        "%{Internals.NSystem}/src",
    }

    links {
        "NSystem"
    }

    defines {
		"_CRT_SECURE_NO_WARNINGS", "GLFW_INCLUDE_NONE"
    }

    defineExport()
    defineApiInfo()

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
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
