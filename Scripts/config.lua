
-- Nkentseu Config

-- builds paths

outputdir 		= ""
build 			= ""
build_int 		= ""

function OutputDir()
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
end

function BuildDir()
    build = "%{wks.location}/build/bin/"
end

function BuildIntDir()
    build_int = "%{wks.location}/build/int/"
end

function BuildsInfos(projectName)
    targetdir ( build .. outputdir .. "/" .. projectName )
    objdir ( build_int .. outputdir .. "/" .. projectName )
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

--VULKAN_SDK = os.getenv("VULKAN_SDK")

Externals = {}

filter "system:windows"
Externals["VkInclude"] = "%{wks.location}/External/Libs/Vulkan-Headers-1.3.272"
Externals["VkLib"] = "C:/VulkanSDK/1.3.224.1"
filter {}
Externals["Glad"] = "%{wks.location}/External/Libs/Glad"
Externals["Stb"] = "%{wks.location}/External/Libs/Stb"


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
    value           = "GAPI_SOFTWARE",
    description     = "ce flag peut prendre les valeur {GAPI_SOFTWARE, GAPI_OPENGL, GAPI_VULKAN, GAPI_DIRECTX11, GAPI_DIRECTX12 ou GAPI_METAL}",
    default         = "GAPI_SOFTWARE",
    category        = "GAPI",
    allowed         = {
        {"GAPI_SOFTWARE", "Software graphic api"},
        {"GAPI_OPENGL", "Opengl graphic api"},
        {"GAPI_VULKAN", "Vulkan graphic api"},
        {"GAPI_DIRECTX11", "Directx11 graphic api"},
        {"GAPI_DIRECTX12", "Directx12 graphic api"},
        {"GAPI_METAL", "Metal graphic api"}
    }
}

function getGraphicApi()
	libtype = _OPTIONS["graphicsapi"]

    if libtype == "GAPI_SOFTWARE" then
        return "NKENTSEU_GAPI_SOFTWARE"
    end

    if libtype == "GAPI_OPENGL" then
        return "NKENTSEU_GAPI_OPENGL"
    end

    if libtype == "GAPI_VULKAN" then
        return "NKENTSEU_GAPI_VULKAN"
    end

    if libtype == "GAPI_DIRECTX11" then
        return "NKENTSEU_GAPI_DIRECTX11"
    end

    if libtype == "GAPI_DIRECTX12" then
        return "NKENTSEU_GAPI_DIRECTX12"
    end

    if libtype == "GAPI_METAL" then
        return "NKENTSEU_GAPI_METAL"
    end
    return ""
end

graphicsapi = getGraphicApi()

function defineApiInfo()
    if getLibraryType() == "StaticLib" then
        defines
		{
			"NKENTSEU_STATIC"
		}
    end

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

        if graphicsapi == "NKENTSEU_GAPI_OPENGL" then
            files {
                "%{Externals.Glad}/**.h",
                "%{Externals.Glad}/**.c",
                "%{Externals.Glad}/**.cpp"
            }

            includedirs {
                "%{Externals.Glad}/include"
            }
        end

        if graphicsapi == "NKENTSEU_GAPI_VULKAN" then
            includedirs {
                "%{Externals.VkInclude}/include",
            }

            libdirs {
                "%{Externals.VkLib}/Lib/"
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

function defineExport()
    defines {
		"NKENTSEU_EXPORTS"
    }
end