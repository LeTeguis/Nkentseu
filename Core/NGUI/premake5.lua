project "NGUI"
    kind (libraryType)
    langageInformations()
    staticruntime "off"

    BuildsInfos("%{prj.name}")

    pchheader "%{prj.name}Pch/ntspch.h"
    pchsource "src/%{prj.name}Pch/ntspch.cpp"
    
    files {
        -- NGUI
        "src/**.h",
        "src/**.hpp",
        "src/**.inl",
        "src/**.cpp"
    }

    includedirs {
        "./src",
        "%{Externals.Stb}/src",
        "%{Internals.NSystem}/src",
        "%{Internals.Logger}/src",
        "%{Internals.Ntsm}/src",
    }

    links {
        "Ntsm",
        "Logger",
        "NSystem",
        "Stb",
    }

    defines {
		"_CRT_SECURE_NO_WARNINGS", "GLFW_INCLUDE_NONE"
    }

    defineExport()
    defineApiInfo()
    -- linkoptions { "-Winvalid-token-paste" }

    filter "system:windows"
        systemversion "latest"
		optimize "off"

        links {
            --"kernel32", 
            --"user32", 
            --"hid", 
            "Xinput"
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
        getLinuxWinApi()

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
