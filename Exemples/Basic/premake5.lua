project "Basic"
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
        "%{Internals.NTSGui}/src",
        "%{Internals.NTSGraphics}/src",
        "%{Internals.NTSWindow}/src",
        "%{Internals.NTSMaths}/src",
        "%{Internals.NTSLogger}/src",
        "%{Internals.NTSCore}/src",
        "%{Externals.FreeType}/include",
    }

    libdirs {
        "%{Externals.FreeType}/lib",
    }

    links {
        "NTSGui",
        "NTSGraphics",
        "NTSWindow",
        "NTSMaths",
        "NTSLogger",
        "NTSCore",
        "freetype",
        "Stb"
    }

    defineApiInfo()
    linksGraphicApi()

    filter "system:windows"
        systemversion "latest"
		optimize "off"
        entrypoint "WinMainCRTStartup"

        PostBuilds("NTSGui", "%{prj.name}")
        PostBuilds("NTSLogger", "%{prj.name}")
        PostBuilds("NTSMaths", "%{prj.name}")
        PostBuilds("NTSCore", "%{prj.name}")
        PostBuilds("NTSGraphics", "%{prj.name}")
        PostBuilds("NTSWindow", "%{prj.name}")
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
