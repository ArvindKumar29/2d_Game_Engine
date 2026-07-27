workspace  "Hazle  Engine"
	architecture "x64"
	startproject "HazleNut"
	staticruntime "on"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]		= "HazleEngine/vendor/GLFW/include"
IncludeDir["glad"]		= "HazleEngine/vendor/glad/include"
IncludeDir["imgui"]		= "HazleEngine/vendor/imgui"
IncludeDir["glm"]		= "HazleEngine/vendor/GLM"
IncludeDir["stb_image"] = "HazleEngine/vendor/stb_image"
IncludeDir["entt"]		= "HazleEngine/vendor/entt/include"
IncludeDir["yaml_cpp"]	= "HazleEngine/vendor/yamlcpp/include"
IncludeDir["imguizmo"]	= "HazleEngine/vendor/imguizmo/src"
group "Dependencies"
	include "HazleEngine/vendor/GLFW"
	include "HazleEngine/vendor/glad"
	include "HazleEngine/vendor/imgui"
	include "HazleEngine/vendor/yamlcpp"

group ""

project "HazleEngine"
	location "HazleEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "HazleEngine/src/Hazle/Core/hzpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/imguizmo/src/ImGuizmo.h",
		"%{prj.name}/vendor/imguizmo/src/ImGuizmo.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.imguizmo}",
		"%{prj.name}/src/Hazle"
	}

	links {
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib",
		"yaml-cpp"
	}
	
	filter "files:HazleEngine/vendor/imguizmo/**.cpp"
	flags {"NoPCH"}

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
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/vendor/GLM/glm/**.h",
		"%{prj.name}/vendor/GLM/glm/**.inl",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs {
		"HazleEngine/vendor/spdlog/include",
		"HazleEngine/vendor/GLFW/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
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

project "HazleNut"
	location "HazleNut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/vendor/GLM/glm/**.h",
		"%{prj.name}/vendor/GLM/glm/**.inl",
		"%{prj.name}/src/**.cpp"
	}

	defines{
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"HazleEngine/vendor/spdlog/include",
		"HazleEngine/vendor/GLFW/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.imguizmo}",
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