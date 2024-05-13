project "Unitest"
    kind (libraryType)
    langageInformations()
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
        "./src",
        "%{Internals.NSystem}/src",
        "%{Internals.Logger}/src",
    }

    links {
        "Logger",
        "NSystem",
    }

    defines {
		"_CRT_SECURE_NO_WARNINGS"
    }

    links {
    }

    defineExport()
	defineApiInfo()

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
        defines {  "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH "}
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
