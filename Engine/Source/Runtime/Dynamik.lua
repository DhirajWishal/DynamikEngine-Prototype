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
		"**.cpp",
		"**.h",
		"Private/**.cpp",
		"Public/**.h",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Source/Runtime/src",
		"$(SolutionDir)Engine/Source/Runtime/GameLibraries",
		"$(SolutionDir)Engine/Source/Shared/Audio/src",
		"$(SolutionDir)Engine/Source/Shared/Intellect/src",
		"$(SolutionDir)Engine/Source/Shared/Events/src",
		"$(SolutionDir)Engine/Source/Shared/Managers/src",
		"$(SolutionDir)Engine/Source/Shared/Networking/src",
		"$(SolutionDir)Dependencies/Programs/DevConsole/src",
		"$(SolutionDir)Dependencies/Programs/Debugger/src",
		"$(SolutionDir)Engine/Source/Internal/Platform/src",
		"$(SolutionDir)Engine/Source/Internal/Utilities/src",
		"$(SolutionDir)Engine/Source/Internal/CentralDataHub/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.gli}",
		DynamikInternalLibsPath
	}

	libdirs {
		"%{IncludeLib.irrKlang}",
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}",
		"%{IncludeLib.Assimp}",
		DynamikInternalBinaries
	}

	links { 
		"Audio",
		"Events",
		"Intellect",
		"Platform",
		"glfw3dll",
		"opengl32",
		"vulkan-1",
		"assimp",
		"assimp-vc140-mt",
		"Debugger",
		"CentralDataHub",
		"Managers",
		"Utilities",
		"Networking",

		"DataTypesLib",
		"MemoryLib",
		"SystemLib",
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
			"DMK_USE_VULKAN",
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