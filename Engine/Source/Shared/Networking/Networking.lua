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
		"$(SolutionDir)Engine/Source/Shared/ADGR/src",
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
		"%{IncludeDir.Vulkan}"
	}

	links { 
		"Utilities",
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
