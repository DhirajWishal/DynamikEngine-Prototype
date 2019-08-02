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
--IncludeDir["GLFW"] = "E:/Programming/Libraries/GLFW/glfw-3.2.1.bin.WIN64/include"
IncludeDir["Glad"] = "$(SolutionDir)libs/Glad/include"
IncludeDir["glm"] = "$(SolutionDir)libs/glm"
IncludeDir["Vulkan"] = "E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Include"	
--Include the Vulkan SDK Include path here

--include "Launcher"
--include "ADGR"

---------- Dynamik Engine project discription ----------
project "Dynamik"
	location "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "dmkafx.h"
	pchsource "Dynamik/src/dmkafx.cpp"

	files {
		"Dynamik/**.txt",
		"Dynamik/src/**.h",
		"Dynamik/src/**.cpp",
		"Dynamik/src/**.txt",
		"Dynamik/GameLibraries/**.cpp",
		"Dynamik/GameLibraries/**.h",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)components/ADGR/",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	}

	links { 
		"ADGR",
		"glfw3dll",
		"opengl32",
		"vulkan-1"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL",
			"_WINDLL",
			"GLFW_INCLUDE_VULKAN",
			"GLFW_INCLUDE_NONE"
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

---------- Application project discription ----------
project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	files {
		"Application/**.h",
		"Application/**.cpp",
		"Application/src/**.h",
		"Application/src/**.cpp"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs",
		"$(solutionDir)components/ADGR",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib",
	}

	links {
		"Dynamik",
		"ADGR",
		"glfw3dll",
		"opengl32",
		"vulkan-1"
	}

	postbuildcommands {
		"{copy} $(SolutionDir)bin\\$(Configuration)-$(Platform)\\ADGR\\ADGR.dll $(SolutionDir)bin\\$(Configuration)-$(Platform)\\Application",
		"{copy} $(SolutionDir)bin\\$(Configuration)-$(Platform)\\Dynamik\\Dynamik.dll $(SolutionDir)bin\\$(Configuration)-$(Platform)\\Application"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_VULKAN"
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

---------- Advanced Dynamic Graphics Renderer project discription ----------
project "ADGR"
	location "components/ADGR"
	kind "SharedLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "adgrafx.h"
	pchsource "components/ADGR/src/adgrafx.cpp"

	files {
		"components/ADGR/**.h",
		"components/ADGR/**.cpp",
		"components/ADGR/src/**.h",
		"components/ADGR/src/**.cpp"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs",
		"$(solutionDir)components/ADGR",
		"$(solutionDir)components/ADGR/src",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	}

	links { 
		"glfw3dll",
		"opengl32",
		"vulkan-1"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL",
			"GLFW_INCLUDE_VULKAN"
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		--runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		--runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		--runtime "Release"
		optimize "On"
