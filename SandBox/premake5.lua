project "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"

	targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

	defines {
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
		"%{prjocation}/../GooE/src",
		"%{prj.location}/../GooE/vendor/spdlog/include",
		"%{vendorInclude.glm}",
		"%{vendorInclude.ImGui}",
		"%{vendorInclude.ImGui}/..",
		"%{vendorInclude.entt}",
	}
