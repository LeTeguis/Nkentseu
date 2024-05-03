
-- Nkentseu Config

-- builds paths

outputdir 		= ""
build 			= ""
build_obj 		= ""

function OutputDir()
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
end

function BuildDir()
    build = "%{wks.location}/build/bin/"
end

function BuildIntDir()
    build_obj = "%{wks.location}/build/obj/"
end

function BuildsInfos(projectName)
    targetdir ( build .. outputdir .. "/" .. projectName )
    objdir ( build_obj .. outputdir .. "/" .. projectName )
end


-- post building

function PostBuilds(libName, projectName)
    if libraryType == "SharedLib" then
        postbuildcommands {
            ("{COPY} " .. build .. outputdir .. "/" .. libName .. "/" .. libName .. ".dll " .. build .. outputdir .. "/" .. projectName),
        }
    end
end


-- External api

VULKAN_SDK = os.getenv("VULKAN_SDK")

Externals = {}

Externals["Glad"] = "%{wks.location}/External/Libs/Glad"
Externals["Stb"] = "%{wks.location}/External/Libs/Stb"
--Externals["VkInclude"] = "%{wks.location}/External/Libs/Vulkan-Headers-1.3.272/include"
Externals["VkInclude"] = VULKAN_SDK .. "/Include"
Externals["VkLib"] = VULKAN_SDK .. "/Lib"
print(Externals.VkInclude)
print(Externals.VkLib)


-- Internal api

Internals = {}

-- core
Internals["Nkentseu"]       = "%{wks.location}/Core/Nkentseu"
Internals["NSystem"]        = "%{wks.location}/Core/NSystem"
Internals["Logger"]         = "%{wks.location}/Core/Logger"
Internals["Unitest"]        = "%{wks.location}/Core/Unitest"
Internals["Ntsm"]           = "%{wks.location}/Core/Ntsm"

-- engine
Internals["Unkeny"]       = "%{wks.location}/Engine/Unkeny"
Internals["UnkenyEditor"]         = "%{wks.location}/Engine/UnkenyEditor"

-- tests
Internals["GlobalTests"]    = "%{wks.location}/Tests/GlobalTests"

-- exemples
Internals["Window"]         = "%{wks.location}/Exemples/Window"


-- Options

-- Startup project configurations
newoption {
    trigger         = "linuxwindowapi",
    value           = "",
    description     = "ce flag peut prendre les valeurs NKENTSEU_PLATFORM_LINUX_XCB ou NKENTSEU_PLATFORM_LINUX_XLIB",
    default         = "NKENTSEU_PLATFORM_LINUX_XCB",
    category        = "WindowApi"
}

function getLinuxWindowApi()
	return _OPTIONS["linuxwindowapi"]
end

newoption {
    trigger         = "startupproject",
    value           = "",
    description     = "ce flag peut prendre comme valeur le nom par defaut d'un projet",
    default         = "",
    category        = "Project"
}

function getStartupProject()
	return _OPTIONS["startupproject"]
end

startupproject = getStartupProject()


-- Library type configuration
newoption {
    trigger         = "libtype",
    value           = "STATIC_LIB",
    description     = "ce flag peut prendre les valeur {STATIC_LIB ou DYNAMIC_LIB}",
    default         = "STATIC_LIB",
    category        = "Lib",
    allowed         = {
        {"STATIC_LIB", "STATIC LIB"},
        {"DYNAMIC_LIB", "DYNAMIC LIB"}
    }
}

function getLibraryType()
	libtype = _OPTIONS["libtype"]

    if libtype == "DYNAMIC_LIB" then
        return "SharedLib"
    else
        return "StaticLib"
    end
end

libraryType = getLibraryType()


-- Graphics api configuration
newoption {
    trigger         = "graphicsapi",
    value           = "NKENTSEU_GRAPHICS_API_SOFTWARE",
    description     = "ce flag peut prendre les valeur {NKENTSEU_GRAPHICS_API_SOFTWARE, NKENTSEU_GRAPHICS_API_OPENGL, NKENTSEU_GRAPHICS_API_VULKAN, NKENTSEU_GRAPHICS_API_DIRECTX11, NKENTSEU_GRAPHICS_API_DIRECTX12 ou NKENTSEU_GRAPHICS_API_METAL}",
    default         = "NKENTSEU_GRAPHICS_API_SOFTWARE",
    category        = "GRAPHICS_API",
    allowed         = {
        {"NKENTSEU_GRAPHICS_API_SOFTWARE", "Software graphic api"},
        {"NKENTSEU_GRAPHICS_API_OPENGL", "Opengl graphic api"},
        {"NKENTSEU_GRAPHICS_API_VULKAN", "Vulkan graphic api"},
        {"NKENTSEU_GRAPHICS_API_DIRECTX11", "Directx11 graphic api"},
        {"NKENTSEU_GRAPHICS_API_DIRECTX12", "Directx12 graphic api"},
        {"NKENTSEU_GRAPHICS_API_METAL", "Metal graphic api"}
    }
}

function getGraphicApi()
	return _OPTIONS["graphicsapi"]
end

graphicsapi = getGraphicApi()

function defineApiInfo()
    if getLibraryType() == "StaticLib" then
        defines
		{
			"NKENTSEU_STATIC"
		}
    end

    filter "system:linux"
            defines { getLinuxWindowApi() }

    filter "configurations:Debug"
        defines {  "NKENTSEU_DEBUG"}

    filter "configurations:Release"
        defines { "NKENTSEU_RELEASE" }

    filter {}
end

function defineGraphicApi()
    if graphicsapi ~= "" then
		defines
		{
			graphicsapi
		}

        if graphicsapi == "NKENTSEU_GRAPHICS_API_OPENGL" then
            includedirs {
                "%{Externals.Glad}/include"
            }

            links {
                "Glad", "opengl32"
            }
        end

        if graphicsapi == "NKENTSEU_GRAPHICS_API_VULKAN" then
            includedirs {
                "%{Externals.VkInclude}/",
            }

            libdirs {
                "%{Externals.VkLib}/"
            }

            filter "system:windows"
                links { "vulkan-1", "VkLayer_utils" }

            filter "system:macosx"
                links { "libvulkan.1", "libMoltenVK", "libshaderc_combined" }

            filter "system:linux"
                links { "libvulkan", "libVkLayer_utils" }
        end
	end
end

function linksGraphicApi()
    if graphicsapi ~= "" then
        if graphicsapi == "NKENTSEU_GRAPHICS_API_OPENGL" then
            links {
                "Glad", "opengl32"
            }
        end

        if graphicsapi == "NKENTSEU_GRAPHICS_API_VULKAN" then
            filter "system:windows"
                links { "vulkan-1", "VkLayer_utils" }

            filter "system:macosx"
                links { "libvulkan.1", "libMoltenVK", "libshaderc_combined" }

            filter "system:linux"
                links { "libvulkan", "libVkLayer_utils" }
        end
	end
end

function defineExport()
    defines {
		"NKENTSEU_EXPORTS"
    }
end

-- Langage informations

function langageInformations()
    language "C++"
    cppdialect "C++20"
    --toolchain "clang"
    --toolset ("clang")
    --toolchainversion "5.0"
    -- buildoptions{ "Waddress-of-temporary" }
end