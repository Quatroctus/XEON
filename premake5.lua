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

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"XEON_PLATFORM_WINDOWS",
			"XEON_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	filter "configurations:Debug*"
		defines "XEON_DEBUG"
		symbols "On"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		optimize "On"
	filter "configurations:Distribution*"
		defines "XEON_DIST"
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
		symbols "On"
	filter "configurations:Release*"
		defines "XEON_RELEASE"
		optimize "On"
	filter "configurations:Distribution*"
		defines "XEON_DIST"
		optimize "On"