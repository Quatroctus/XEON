project "glad"
	kind "StaticLib"
	language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs {
		"include"
	}

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug*"
		defines "XEON_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		runtime "Release"
		optimize "on"
