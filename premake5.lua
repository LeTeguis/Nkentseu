include "Scripts/config.lua"

workspace "Nkentseu"
	architecture "x86_64"
	startproject (startupproject)

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "configurations:Debug"
		defines {  "_DEBUG" }
	filter{}

OutputDir()
BuildDir()
BuildIntDir()

group "External"
	include "External/Libs/Stb"
	include "External/Libs/Glad"
	include "External/Libs/GLM"
	--include "External/Libs/SPIRV-Cross"
	--include "External/Libs/GLSlang"
group ""

group "Core"
	include "Core/NSystem"
	include "Core/Logger"
	include "Core/Nkentseu"
	include "Core/Unitest"
	include "Core/Ntsm"
	include "Core/NGUI"


	include "Core/NTSCore"
	include "Core/NTSLogger"
	include "Core/NTSMaths"
	include "Core/NTSUnitest"
	include "Core/NTSWindow"
	include "Core/NTSGraphics"
	include "Core/NTSGui"
group ""

group "Engine"
	include "Engine/Unkeny"
	include "Engine/UnkenyEditor"
group ""

group "Exemples"
	include "Exemples/Window"
	include "Exemples/Basic"
group ""

group "Tests"
	include "Tests/GlobalTests"
group ""