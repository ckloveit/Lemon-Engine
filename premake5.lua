workspace "LemonEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}
	startproject "Editor"
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
ThirdPartyIncludeDir = {}
ThirdPartyIncludeDir["imgui"] = "%{wks.location}/Lemon/ThirdParty/imgui"
ThirdPartyIncludeDir["glm"] = "%{wks.location}/Lemon/ThirdParty/glm"
ThirdPartyIncludeDir["std_image"] = "%{wks.location}/Lemon/ThirdParty/std_image"

group "External"
	include "Lemon/ThirdParty/imgui"
group ""

project "Lemon"
	location "Lemon"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "LemonPCH.h"
	pchsource "Lemon/Src/LemonPCH.cpp"
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/Src/**.h",
		"%{prj.name}/ThirdParty/glm/glm/**.hpp",
		"%{prj.name}/ThirdParty/glm/glm/**.inl",
		"%{prj.name}/ThirdParty/std_image/*.h",
		"%{prj.name}/ThirdParty/std_image/*.cpp",
	}

	links 
	{
		"imgui",
	}
	
	includedirs
	{
		"%{prj.location}/Src",
		"%{prj.location}/ThirdParty/spdlog/include",
		"%{prj.location}/ThirdParty",
		"%{ThirdPartyIncludeDir.GLFW}",
		"%{ThirdPartyIncludeDir.GLAD}",
		"%{ThirdPartyIncludeDir.glm}",
		"%{ThirdPartyIncludeDir.std_image}",
		"%{ThirdPartyIncludeDir.entt}",
		"%{ThirdPartyIncludeDir.yaml_cpp}",
		"%{ThirdPartyIncludeDir.ImGuizmo}"
	}

	filter "system:Windows"
		systemversion "latest" -- To use the latest version of the SDK available

		defines
		{
			"LEMON_PLATFORM_WINDOW",
			"LEMON_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"LEMON_GRAPHICS_D3D11"
		}

	--	postbuildcommands
	--	{
	--		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	--	}

	filter "configurations:Debug"
		defines "LEMON_DEBUG"
		buildoptions "/MDd"
	    symbols "On"
	filter "configurations:Release"
		defines "LEMON_RELEASE"
		buildoptions "/MD"
	    optimize "On"
	filter "configurations:Shipping"
		defines "LEMON_SHIPPING"
		buildoptions "/MD"
        optimize "On"

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Lemon/ThirdParty/spdlog/include",
		"%{wks.location}/Lemon/Src",
		"%{ThirdPartyIncludeDir.glm}",
		"%{wks.location}/Lemon/ThirdParty",
	}

	links
	{
		"Lemon"
	}

	filter "system:Windows"
		systemversion "latest" -- To use the latest version of the SDK available

		defines
		{
			"LEMON_PLATFORM_WINDOW",
			"LEMON_GRAPHICS_D3D11"
		}

	filter "configurations:Debug"
		defines "LEMON_DEBUG"
		buildoptions "/MDd"
	    symbols "On"
	filter "configurations:Release"
		defines "LEMON_RELEASE"
		buildoptions "/MD"
	    optimize "On"
	filter "configurations:Shipping"
		defines "LEMON_SHIPPING"
		buildoptions "/MD"
        optimize "On"