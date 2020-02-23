---------- Dynamik Engine project description ----------

project "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "dmkafx.h"
	pchsource "Runtime/src/dmkafx.cpp"

	files {
		"Runtime/**.txt",
		"Runtime/**.lua",
		"Runtime/**.hint",
		"Runtime/src/**.h",
		"Runtime/src/**.cpp",
		"Runtime/src/**.txt",
		"Runtime/GameLibraries/**.cpp",
		"Runtime/GameLibraries/**.h",
		"$(SolutionDir)Dependencies/glm/**.hpp",
		"$(SolutionDir)Dependencies/glm/**.inl",
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