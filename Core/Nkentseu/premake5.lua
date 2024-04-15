project "Nkentseu"
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
        "./src",
        -- "%{Externals.Stb}/include",
        "%{Internals.NSystem}/src",
        "%{Internals.Logger}/src",
        "%{Internals.Ntsm}/src",
    }

    links {
        "NSystem",
        "Logger",
        "Ntsm",
        -- "Stb"
    }

    defines {
		"_CRT_SECURE_NO_WARNINGS", "GLFW_INCLUDE_NONE"
    }

    defineExport()
    defineApiInfo()
    -- defineGraphicApi()

    links {
    }

    --removefiles{
    --    "./src/Nkentseu/Platform/Window/**", 
    --    "./src/Nkentseu/Platform/Main/**"
    --}

    filter "system:windows"
        systemversion "latest"
		optimize "off"

        links {
        }
        --files{
        --    "./src/Nkentseu/Platform/Window/Windows/**", 
        --    "./src/Nkentseu/Platform/Main/Main.h", 
        --    "./src/Nkentseu/Platform/Main/WindowsMain.h"
        --}

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
            "X11"
        }
        --files{
        --    "./src/Nkentseu/Platform/Window/Linux/**", 
        --    "./src/Nkentseu/Platform/Main/Main.h", 
        --    "./src/Nkentseu/Platform/Main/LinuxMain.h"
        --}

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