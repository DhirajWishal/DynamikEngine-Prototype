---------- Developer Console project ----------

project "DevCon"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(ProjectName)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "dcnsafx.h"
	pchsource "DevConsole/src/dcnsafx.cpp"

	files {
		"**.cpp",
		"**.h",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Source/Runtime/src",
		"$(SolutionDir)Engine/Source/Runtime/GameLibraries",

		"$(SolutionDir)Engine/Source/Shared/ADGR/src",
		"$(SolutionDir)Engine/Source/Shared/Audio/src",
		"$(SolutionDir)Engine/Source/Shared/Audio/src",
		"$(SolutionDir)Engine/Source/Shared/Managers/src",
		"$(SolutionDir)Engine/Source/Internal/Platform/src",
		"$(SolutionDir)Dependencies/Programs/DevConsole/src",
		"$(SolutionDir)Dependencies/Programs/Debugger/src",
		"$(SolutionDir)Engine/Source/Internal/CentralDataHub/src",
		"$(SolutionDir)Engine/Source/Shared/Networking/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}"
	}

	links {
		"Dynamik",
		"Debugger"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS"
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