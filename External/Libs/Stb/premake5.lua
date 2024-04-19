project "Stb"
    kind (libraryType)
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    BuildsInfos("%{prj.name}")

    files {
        --"./src/**.h",
        --"./src/**.hpp",
        --"./src/**.c",
        --"./src/**.cpp",
        "./include/**.h",
        "./include/**.hpp",
        "./include/**.c",
        "./include/**.cpp",
    }

    includedirs {
        "./include"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
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

    filter "system:linux"
        links {
        }

        -- Ajout d'options de compilation pour toutes les configurations sous Linux
        buildoptions { "-fPIC", "-pthread" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter {}
