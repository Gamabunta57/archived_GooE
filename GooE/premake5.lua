project "GooE"
	kind "StaticLib"
	staticruntime "on"

	targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "src/gepch.cpp"

	defines {
		"GOOE_BUILD_STATIC",
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	files {
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/vendor/stb_image/stb_image.h",
		"%{prj.location}/vendor/stb_image/stb_image.cpp",

		"%{prj.location}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.location}/vendor/ImGuizmo/ImGuizmo.cpp",
	}

	includedirs {
		"%{prj.location}/src",
		"%{prj.location}/vendor/spdlog/include",
		"%{vendorInclude.GLFW}",
		"%{vendorInclude.glad}",
		"%{vendorInclude.ImGui}",
		"%{vendorInclude.glm}",
		"%{vendorInclude.stb}",
		"%{vendorInclude.entt}",
		"%{vendorInclude.yamlcpp}",
		"%{vendorInclude.imguizmo}",
	}

	links {
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags {"NoPCH"}
