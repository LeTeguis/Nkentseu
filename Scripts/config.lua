
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

Externals["Glm"] = "%{wks.location}/External/Libs/GLM"
Externals["Glad"] = "%{wks.location}/External/Libs/Glad"
Externals["Stb"] = "%{wks.location}/External/Libs/Stb"
--Externals["SPIRVCross"] = "%{wks.location}/External/libs/SPIRV-Cross/"
--Externals["Glslang"] = "%{wks.location}/External/Libs/GLSlang"
--Externals["VkInclude"] = "%{wks.location}/External/Libs/Vulkan-Headers-1.3.272/include"
Externals["VkInclude"] = VULKAN_SDK .. "/Include"
Externals["VkLib"] = VULKAN_SDK .. "/Lib"

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

-- Linux use WSL

newoption {
    trigger         = "linuxusewsl",
    value           = "NKENTSEU_LINUX_USE_WSL",
    description     = "ce flag peut prendre les valeur {NKENTSEU_LINUX_USE_WSL, NKENTSEU_LINUX_UNUSE_WSL}",
    default         = "NKENTSEU_LINUX_USE_WSL",
    category        = "Platform",
    allowed         = {
        {"NKENTSEU_LINUX_USE_WSL", "use WSL"},
        {"NKENTSEU_LINUX_UNUSE_WSL", "unuse WSL"}
    }
}

function getWSLPlatform()
	linux_use_wsl = _OPTIONS["linuxusewsl"]

    defines {
        linux_use_wsl
    }
end

-- Linux window api

newoption {
    trigger         = "linuxwinapi",
    value           = "NKENTSEU_LINUX_WIN_API_XLIB",
    description     = "ce flag peut prendre les valeur {NKENTSEU_LINUX_WIN_API_XLIB, NKENTSEU_LINUX_WIN_API_XCB}",
    default         = "NKENTSEU_LINUX_WIN_API_XLIB",
    category        = "GRAPHICS_API",
    allowed         = {
        {"NKENTSEU_LINUX_WIN_API_XLIB", "XLIB Window manager for linux"},
        {"NKENTSEU_LINUX_WIN_API_XCB", "XCB Window manager for linux"}
    }
}

function getLinuxWinApi()
	linux_win_api = _OPTIONS["linuxwinapi"]

    defines {
        linux_win_api
    }
    

    if linux_win_api == "NKENTSEU_LINUX_WIN_API_XCB" then
        links {
            "X11", "Xcursor", "Xrandr", "Xfixes", "X11-xcb", "xcb", "xcb-util", "xcb-icccm", "xcb-keysyms"
        }
    else
        links {
            "X11", "Xcursor", "Xrandr", "Xfixes"
        }
    end

    getWSLPlatform()
end

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
        desactivePIC()
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

        --if graphicsapi == "NKENTSEU_GRAPHICS_API_OPENGL" then
            includedirs {
                "%{Externals.Glad}/include"
            }

            links {
                "Glad"
            }
            
            filter "system:windows"
                links { "opengl32" }

            filter "system:macosx"
                links { "GL" }

            filter "system:linux"
                links { "GL" }

            filter {}
        --elseif graphicsapi == "NKENTSEU_GRAPHICS_API_VULKAN" then
            includedirs {
                "%{Externals.VkInclude}/",
                --"%{Externals.Glslang}",
                --"%{Externals.Glslang}/glslang/Include",
                --"%{Externals.Glslang}/StandAlone",
                --"%{Externals.SPIRVCross}/src",
                --"%{Externals.SPIRVCross}/include",
            }

            libdirs {
                "%{Externals.VkLib}/",
                --"GLSlang",
                --"SPIRVCross",
            }

            filter "system:windows"
                links { "vulkan-1", "VkLayer_utils" }

            filter "system:macosx"
                links { "libvulkan.1", "libMoltenVK", "libshaderc_combined" }

            filter "system:linux"
                links {"vulkan"}

            filter {}
        --end
	end
end

function linksGraphicApi()
    if graphicsapi ~= "" then
        --if graphicsapi == "NKENTSEU_GRAPHICS_API_OPENGL" then
            links {
                "Glad"
            }
            filter "system:windows"
                links { "opengl32" }

            filter "system:macosx"
                links { "GL" }

            filter "system:linux"
                links { "GL" }

            filter {}
        --elseif graphicsapi == "NKENTSEU_GRAPHICS_API_VULKAN" then
            libdirs {
                "%{Externals.VkLib}/",
                --"GLSlang",
                --"SPIRVCross",
            }
            
            filter "system:windows"
                links { "vulkan-1", "VkLayer_utils" }

            filter "system:macosx"
                links { "libvulkan.1", "libMoltenVK", "libshaderc_combined" }

            filter "system:linux"
                links {"vulkan"}
        --end
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

    -- toolset ("clang")

    filter "system:linux"
        buildoptions {}
        --toolchain "clang"
        toolset ("clang")
        --toolchainversion "5.0"
        -- buildoptions{ "Waddress-of-temporary" }

    filter {}
end

function desactivePIC()
    filter "system:linux"
        buildoptions {}
        --buildoptions {"-lbinaryen -lwasm -last -lasmjs -lsupport -lpasses"}
        --buildoptions { "-Wl,--allow-shlib-undefined" }
        --buildoptions { "-fno-PIC" }
    filter {}
end