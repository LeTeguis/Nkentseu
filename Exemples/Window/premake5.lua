project "Window"
    kind "ConsoleApp"

    langageInformations()
    staticruntime "off"

    BuildsInfos("%{prj.name}")

    files {
        "src/**.h",
        "src/**.hpp",
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
        "Nkentseu",
        "Ntsm",
        "Logger",
        "NSystem",
        "Stb"
    }

    defineApiInfo()
    linksGraphicApi()

    filter "system:windows"
        systemversion "latest"
		optimize "off"
        entrypoint "WinMainCRTStartup"

        PostBuilds("Nkentseu", "%{prj.name}")
        PostBuilds("Logger", "%{prj.name}")
        PostBuilds("Ntsm", "%{prj.name}")
        PostBuilds("NSystem", "%{prj.name}")
        -- PostBuilds("Stb", "%{prj.name}")
    
    filter "system:macosx"

    filter "system:linux"
        getLinuxWinApi()

    filter "configurations:Debug"
        defines { "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
