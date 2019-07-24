workspace "Dynamik"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

outputDir = "$(Configuration)-$(Platform)"
solutionDir = "$(SolutionDir)"
configurationMacro = "$(Configuration)"
platformMacro = "$(Platform)"
projectNameMacro = "$(ProjectName)"

includePath =  "$(SolutionDir)Dynamik/lib/spdlog/include"

IncludeDir = {}
IncludeDir["GLFW"] = "$(SolutionDir)libs/GLFW/include"
IncludeDir["Glad"] = "$(SolutionDir)libs/Glad/include"
IncludeDir["glm"] = "$(SolutionDir)libs/glm"
--IncludeDir["Vulkan"] = "E:\Programming\Codes\Game Development\Libraries\VulkanSDK\1.1.108.0\Include"	
--Include the Vulkan SDK Include path here

--include "Launcher"

project "Dynamik"
	location "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "dmkafx.h"
	pchsource "Dynamik/src/dmkafx.cpp"

	files {
		"Dynamik/src/**.h",
		"Dynamik/src/**.cpp",
		"Dynamik/src/**.txt",
		"Dynamik/GameLibraries/**.cpp",
		"Dynamik/GameLibraries/**.h",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		-- "%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links { 
		--"glfw3",
		--"opengl32",
		--"vulkan-1"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL",
			"_WINDLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			"{COPY} $(SolutionDir)bin/$(Configuration)-$(Platform)Dynamik/Dynamik.dll $(SolutionDir)bin/$(Configuration)-$(Platform)Application/"
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		optimize "On"

project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)-bin")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)-obj")

	files {
		"Application/src/**.h",
		"Application/src/**.cpp"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs"
	}

	links {
		"Dynamik"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		optimize "On"

project "Launcher"
	location "Launcher"
	kind "WindowedApp"
	language "C#"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	files {
		"Launcher/**.cs",
		"Launcher/**.config"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG", "TRACE" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "TRACE" }
		optimize "On"

	filter "configurations:Distribution"
		optimize "On"