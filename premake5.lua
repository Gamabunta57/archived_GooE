workspace "GooE"
    language "C++"
    cppdialect "c++17"
	startproject "Sandbox"
    configurations {"Debug", "Release"}
    platforms {"Win64"}

    filter "configurations:Debug"
        defines {"DEBUG"}

    filter "configurations:Release"
        defines {"Release"}
        optimize "On"

    filter "platforms:Win64"
        defines {"WIN_64"}
        system "Windows"
        architecture "x86_64"
        systemversion "latest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "GooE"
include "Sandbox"