outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GLM"
    kind "StaticLib"
    langageInformations()
    staticruntime "off"

    BuildsInfos("%{prj.name}")

    files {
        "./glm/**.h",
        "./glm/**.hpp",
        "./glm/**.cpp",
        "./glm/**.c",
        "./glm/**.inl",
        "./util/**.natvis"
    }

    includedirs {
        "./"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    filter "system:windows"
        systemversion "latest"

    filter "system:macosx"
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["USeModernBuildSystem"] = "NO"
        }

        -- Ajout d'options de compilation pour toutes les configurations sous macOS
        buildoptions { "-stdlib=libc++", "-fPIC", "-pthread" }

    filter "system:linux"
        -- Ajout d'options de compilation pour toutes les configurations sous Linux
        buildoptions { "-fPIC", "-pthread" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
