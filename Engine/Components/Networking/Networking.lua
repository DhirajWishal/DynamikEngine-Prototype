---------- Networking project ----------

project "Networking"
	kind "SharedLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "netafx.h"
	pchsource "src/netafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
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
		"Platform",
		"Managers",
		"CentralDataHub",
		"Debugger"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL"
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
