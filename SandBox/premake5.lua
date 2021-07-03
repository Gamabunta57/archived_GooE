project "Sandbox"
    kind "ConsoleApp"
    staticruntime "off"

    targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

    defines {
        "GOOE_PLATFORM_WINDOWS",
        "GOOE_BUILD_STATIC"
    }

    files {
        "src/**.h",
		"src/**.cpp",
    }

    links {
        "GooE"
    }

	includedirs
	{
		"src",
        "%{prjocation}/../GooE/src"
	}