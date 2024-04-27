outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Stb"
    kind (libraryType)
    langageInformations()
    staticruntime "off"

    targetdir ( build .. outputdir .. "/%{prj.name}" )
    objdir ( build_int .. outputdir .. "/%{prj.name}" )

    files {
        "./src/**.h",
        "./src/**.hpp",
        "./src/**.c",
        "./src/**.cpp",
        --"./include/**.h",
        --"./include/**.hpp",
        --"./include/**.c",
        --"./include/**.cpp",
    }

    includedirs {
        "./src"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    filter "system:windows"
        systemversion "latest"

    filter "system:macosx"
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["USeModernBuildSystem"] = "NO"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
