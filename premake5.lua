workspace  "Hazle  Engine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	startproject "HazleNut"
	staticruntime "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "HazleEngine/external-libraries/GLFW/include"
IncludeDir["glad"] = "HazleEngine/external-libraries/glad/include"
IncludeDir["imgui"] = "HazleEngine/external-libraries/imgui"
IncludeDir["glm"] = "HazleEngine/external-libraries/GLM"
IncludeDir["stb_image"] = "HazleEngine/external-libraries/stb_image"
IncludeDir["entt"] = "HazleEngine/external-libraries/entt/include"
IncludeDir["yaml_cpp"] = "HazleEngine/external-libraries/yamlcpp/include"
IncludeDir["imguizmo"] = "HazleEngine/external-libraries/imguizmo"

group "Dependencies"
	include "HazleEngine/external-libraries/GLFW"
	include "HazleEngine/external-libraries/glad"
	include "HazleEngine/external-libraries/imgui"
	include "HazleEngine/external-libraries/yamlcpp"
group ""

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
		"%{prj.name}/external-libraries/stb_image/**.h",
		"%{prj.name}/external-libraries/stb_image/**.cpp",
		"%{prj.name}/external-libraries/imguizmo/ImGuizmo.h",
		"%{prj.name}/external-libraries/imguizmo/ImGuizmo.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"%{prj.name}/external-libraries/spdlog/include",
		"%{prj.name}/external-libraries/glad/include",
		"%{prj.name}/external-libraries/glm",
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
	
	filter "files:HazleEngine/external-libraries/imguizmo/**.cpp"
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
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/external-libraries/GLM/glm/**.h",
		"%{prj.name}/external-libraries/GLM/glm/**.inl",
		"%{prj.name}/src/**.cpp"
	}

	defines{
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"HazleEngine/external-libraries/spdlog/include",
		"HazleEngine/external-libraries/GLFW/include",
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