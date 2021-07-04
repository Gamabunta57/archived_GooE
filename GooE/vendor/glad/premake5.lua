project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/src/glad.c",
    }

    includedirs {
        "%{prj.location}/include"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter {"system:windows", "configurations:Release"}
        buildoptions "/MT"