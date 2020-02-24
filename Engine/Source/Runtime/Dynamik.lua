---------- Dynamik Engine project description ----------

project "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "dmkafx.h"
	pchsource "Runtime/src/dmkafx.cpp"

	files {
		"**.txt",
		"**.lua",
		"**.hint",
		"src/**.h",
		"src/**.cpp",
		"src/**.txt",
		"GameLibraries/**.cpp",
		"GameLibraries/**.h",
		"$(SolutionDir)Dependencies/glm/**.hpp",
		"$(SolutionDir)Dependencies/glm/**.inl",
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Source/Runtime/src",
		"$(SolutionDir)Engine/Source/Runtime/GameLibraries",
		"$(SolutionDir)Engine/Source/Components/ADGR",
		"$(SolutionDir)Engine/Source/Components/ADGR/src",
		"$(SolutionDir)Engine/Source/Components/Audio/src",
		"$(SolutionDir)Engine/Source/Components/Events/src",
		"$(SolutionDir)Engine/Source/Components/Managers/src",
		"$(SolutionDir)Engine/Source/Components/Platform/src",
		"$(SolutionDir)Engine/Source/Components/DevConsole/src",
		"$(SolutionDir)Engine/Source/Components/Debugger/src",
		"$(SolutionDir)Engine/Source/Components/CentralDataHub/src",
		"$(SolutionDir)Engine/Source/Components/Networking/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"%{IncludeLib.irrKlang}",
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
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