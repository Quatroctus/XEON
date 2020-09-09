workspace "XEON"
	configurations {
		"Debug32",
		"Release32",
		"Distribution32",
		"Debug64",
		"Release64",
		"Distribution64"
	}
	startproject "Sandbox"
	filter "configurations:**32"
		architecture "x86"
	filter "configurations:**64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDirs["glad"] = "Engine/vendor/glad/include"
IncludeDirs["imgui"] = "Engine/vendor/imgui"
IncludeDirs["glm"] = "Engine/vendor/glm"
IncludeDirs["stb_image"] = "Engine/vendor/stb_image"

group "Dependencies"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/glad"
	include "Engine/vendor/imgui"

group ""

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "xeonpch.h"
	pchsource "Engine/src/xeonpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%[prj.name}/vendor/glm/glm/**.hpp",
		"%[prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"Engine/src",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.stb_image}"
	}

	links {
		"GLFW",
		"glad",
		"imgui"
	}

	filter "configurations:*32"
		links {
			"$(SolutionDir)/Libraries/Vulkan/Lib32/vulkan-1.lib"
		}
	filter "configurations:*x86_64"
		links {
			"$(SolutionDir)/Libraries/Vulkan/Lib/vulkan-1.lib"
		}

	filter "system:windows"
        systemversion "latest"

		defines {
			"XEON_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug*"
		defines "XEON_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Distribution*"
		defines "XEON_DIST"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Engine/vendor/spdlog/include",
		"Engine/src",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.imgui}"
	}

	links {
		"Engine"
	}

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
	filter "configurations:Distribution*"
		defines "XEON_DIST"
		optimize "on"