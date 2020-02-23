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
		"$(SolutionDir)Engine/Components/ADGR",
		"$(SolutionDir)Engine/Components/ADGR/src",
		"$(SolutionDir)Engine/Components/Audio/src",
		"$(SolutionDir)Engine/Components/Events/src",
		"$(SolutionDir)Engine/Components/Managers/src",
		"$(SolutionDir)Engine/Components/Platform/src",
		"$(SolutionDir)Engine/Components/DevConsole/src",
		"$(SolutionDir)Engine/Components/Debugger/src",
		"$(SolutionDir)Engine/Components/CentralDataHub/src",
		"$(SolutionDir)Engine/Components/Networking/src",
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
