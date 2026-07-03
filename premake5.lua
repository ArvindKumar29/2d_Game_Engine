workspace  "Hazle  Engine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	startproject "Sandbox"
	-- staticruntime "Off"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "HazleEngine/external-libraries/GLFW/include"
IncludeDir["glad"] = "HazleEngine/external-libraries/glad/include"
IncludeDir["imgui"] = "HazleEngine/external-libraries/imgui"
IncludeDir["glm"] = "HazleEngine/external-libraries/GLM"
IncludeDir["stb_image"] = "HazleEngine/external-libraries/stb_image"

include "HazleEngine/external-libraries/GLFW"
include "HazleEngine/external-libraries/glad"
include "HazleEngine/external-libraries/imgui"

project "HazleEngine"
	location "HazleEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "HazleEngine/src/Hazle/Core/hzpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		--"external-libraries/imgui/*.cpp",
		--"external-libraries/imgui/*.h",",
		"%{prj.name}/external-libraries/stb_image/**.h",
		"%{prj.name}/external-libraries/stb_image/**.cpp",
	
		--"{prj.name}/external-libraries/imgui/backends/imgui_impl_glfw.cpp",
		--"{prj.name}/external-libraries/imgui/backends/imgui_impl_opengl3.cpp"
	}

	includedirs {
		"%{prj.name}/external-libraries/spdlog/include",
		"%{prj.name}/external-libraries/glad/include",
		-- "%{prj.name}/external-libraries/imgui",
		"%{prj.name}/external-libraries/glm",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		-- "%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.stb_image}",
		"%{prj.name}/src/Hazle"
	}

	links {
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib",
		--"stb_image"
	}
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_GLAD",
			"_CRT_SECURE_NO_WARNINGS"
		}

		disablewarnings {
			"4005",
			"4996"
		}

	filter "configurations:Debug" 
		defines "HZ_DEBUG"
		symbols "On"
	filter  "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"
	filter  "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/external-libraries/GLM/glm/**.h",
		"%{prj.name}/external-libraries/GLM/glm/**.inl",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs {
		"HazleEngine/external-libraries/spdlog/include",
		"HazleEngine/external-libraries/GLFW/include",
		"%{IncludeDir.glm}",
		"HazleEngine/src",
		"%{IncludeDir.imgui}"
	}
	
	links {
		"HazleEngine"
	}
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		buildoptions{ "/utf-8" }

		defines {
			"HZ_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug" 
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter  "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter  "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"

