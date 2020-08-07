workspace "XEON"
	configurations {
		"Debug32",
		"Release32",
		"Distribution32",
		"Debug64",
		"Release64",
		"Distribution64"
	}
	filter "configurations:*32"
		architecture "x86"
	filter "configurations:*64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDirs["glad"] = "Engine/vendor/glad/include"
IncludeDirs["imgui"] = "Engine/vendor/imgui"

include "Engine/vendor/GLFW"
include "Engine/vendor/glad"
include "Engine/vendor/imgui"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "xeonpch.h"
	pchsource "Engine/src/xeonpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"Engine/src",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.imgui}"
	}

	links {
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib"
	}

	filter "configurations:*32"
		links {
			"E:/Programming/Personal/C++/libraries/VulkanAPI/Lib32/vulkan-1.lib"
		}
	filter "configurations:*x86_64"
		links {
			"E:/Programming/Personal/C++/libraries/VulkanAPI/Lib/vulkan-1.lib"
		}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"XEON_PLATFORM_WINDOWS",
			"XEON_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	filter "configurations:Debug*"
		defines "XEON_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		buildoptions "/MD"
		optimize "On"
	filter "configurations:Distribution*"
		defines "XEON_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Engine/vendor/spdlog/include",
		"Engine/src"
	}

	links {
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"XEON_PLATFORM_WINDOWS"
		}
	filter "configurations:Debug*"
		defines "XEON_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		buildoptions "/MD"
		optimize "On"
	filter "configurations:Distribution*"
		defines "XEON_DIST"
		buildoptions "/MD"
		optimize "On"