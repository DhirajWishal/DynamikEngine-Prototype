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
--Include the Vulkan SDK Include path here
IncludeDir["Vulkan"] = "$(SolutionDir)libs/Vulkan/include"
IncludeDir["boost"] = "E:/Programming/Modules and Runtime/boost_1_70_0"
IncludeDir["gil"] = "$(SolutionDir)libs/gil/boost"
IncludeDir["jpeg"] = "$(SolutionDir)libs/gil/jpeg-6b"
IncludeDir["irrKlang"] = "$(SolutionDir)/libs/irrKlang/include"
IncludeDir["assimp"] = "$(SolutionDir)/libs/assimp"

IncludeLib = {}
--Include the GLFW library path here
IncludeLib["GLFW"] = "$(SolutionDir)libs/GLFW/lib-vc2019"
--Include the Vulkan SDK path here
IncludeLib["Vulkan"] = "$(SolutionDir)libs/Vulkan/lib"
IncludeLib["boostD"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/libs/iostreams/build/msvc-14.2/debug/address-model-64/link-static/threading-multi"
IncludeLib["boostR"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/libs/iostreams/build/msvc-14.2/release/address-model-64/link-static/threading-multi"

include "Launcher"
include "Application"
include "components/ADGR"
include "components/Events"
include "components/DevConsole"
include "components/Audio"
include "components/Platform"
include "components/Debugger"
include "components/CentralDataHub"
include "components/Networking"
include "components/Managers"

---------- Dynamik Engine project description ----------
project "Dynamik"
	location "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
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
		"$(SolutionDir)components/Audio/src",
		"$(SolutionDir)components/Events/src",
		"$(SolutionDir)components/Managers/src",
		"$(SolutionDir)components/Platform/src",
		"$(SolutionDir)components/DevConsole/src",
		"$(SolutionDir)components/Debugger/src",
		"$(SolutionDir)components/CentralDataHub/src",
		"$(SolutionDir)components/Networking/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.Vulkan}"
	}

	links { 
		"ADGR",
		"Audio",
		"Events",
		"Platform",
		"glfw3dll",
		"opengl32",
		"vulkan-1",
		"Debugger",
		"CentralDataHub",
		"Networking"
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