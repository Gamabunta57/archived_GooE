project "GooE"
    kind "StaticLib"
	staticruntime "off"

    targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

    pchheader "gepch.h"
    pchsource "src/gepch.cpp"

    defines {
        "GOOE_PLATFORM_WINDOWS",
        "GOOE_BUILD_STATIC"
    }

    files {
        "src/**.h",
		"src/**.cpp",
    }

    includedirs {
        "%{prj.location}/src",
        "%{prj.location}/vendor/spdlog/include",
        "%{vendorInclude.GLFW}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }
