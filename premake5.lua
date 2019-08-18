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

IncludeDir = {}
IncludeDir["GLFW"] = "$(SolutionDir)libs/GLFW/include"
IncludeDir["Glad"] = "$(SolutionDir)libs/Glad/include"
IncludeDir["glm"] = "$(SolutionDir)libs/glm"
IncludeDir["stb"] = "$(SolutionDir)libs/stb-master"
IncludeDir["tol"] = "$(SolutionDir)libs/tinyobjloader-master"
IncludeDir["spdlog"] = "$(SolutionDir)Dynamik/lib/spdlog/include"
IncludeDir["Vulkan"] = "E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Include"
--Include the Vulkan SDK Include path here
IncludeDir["boost"] = "E:/Programming/Modules and Runtime/boost_1_70_0"
IncludeDir["gil"] = "$(SolutionDir)libs/gil/boost"
IncludeDir["jpeg"] = "$(SolutionDir)libs/gil/jpeg-6b"

--include "Launcher"
include "components/ADGR"
include "components/Events"
include "components/DevConsole"
include "components/Audio"

-- test build include
include "components/TestBuild"

---------- Dynamik Engine project description ----------
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
		"$(SolutionDir)components/ADGR",
		"$(SolutionDir)components/ADGR/src",
		"$(SolutionDir)components/Events/src",
		"$(SolutionDir)components/DevConsole/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	}

	links { 
		"ADGR",
		"Events",
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
		--buildoptions "\\MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		--buildoptions "\\MD"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		--buildoptions "\\MD"
		optimize "On"

---------- Application project description ----------
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
		"$(solutionDir)components/ADGR/src",
		"$(solutionDir)components/Events/src",
		"$(solutionDir)components/DevConsole/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib",
	}

	links {
		"Dynamik",
		"ADGR",
		"Events",
		"glfw3dll",
		"opengl32",
		"vulkan-1"
	}

	postbuildcommands {
		"{copy} $(SolutionDir)bin\\$(Configuration)-$(Platform)\\Sampler\\Sampler.dll $(SolutionDir)bin\\$(Configuration)-$(Platform)\\Application",
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
		--buildoptions "/MTd"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		--buildoptions "/MT"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		--buildoptions "/MT"
		optimize "On"

---------- Sampler project description ----------
project "Sampler"
	location "components/Sampler"
	kind "SharedLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "smpafx.h"
	pchsource "components/Sampler/src/smpafx.cpp"

	files {
		"components/Sampler/**.h",
		"components/Sampler/**.cpp",
		"components/Sampler/src/**.h",
		"components/Sampler/src/**.cpp"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs",
		"$(solutionDir)components/Sampler",
		"$(solutionDir)components/Sampler/src",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
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

---------- Managers library ----------
project "Managers"
	location "components/Managers"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "mngafx.h"
	pchsource "components/Managers/src/mngafx.cpp"

	files {
		"components/Managers/**.h",
		"components/Managers/**.cpp",
		"components/Managers/src/**.h",
		"components/Managers/src/**.cpp"
	}

	includedirs {
		--"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		--"$(SolutionDir)Dynamik/GameLibraries",
		--"$(SolutionDir)lib/libs",
		--"$(solutionDir)components/Sampler",
		--"$(solutionDir)components/Sampler/src",
		"$(solutionDir)components/Managers/src",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		--"$(SolutionDir)libs/GLFW/lib-vc2019",
		--"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	}

	links { 
		--"glfw3dll",
		--"opengl32",
		--"vulkan-1"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			--"GLFW_INCLUDE_VULKAN"
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		runtime "Release"
		optimize "On"
