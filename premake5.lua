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

group "Core"
	include "Core/NSystem"
	include "Core/Logger"
	include "Core/Nkentseu"
	include "Core/Unitest"
	include "Core/Ntsm"
group ""

group "Engine"
	include "Engine/Unkeny"
	include "Engine/UnkenyEditor"
group ""

group "External"
	--include "External/Libs/Stb"
group ""

group "Tests"
	include "Tests/GlobalTests"
group ""

group "Exemples"
	include "Exemples/Window"
group ""