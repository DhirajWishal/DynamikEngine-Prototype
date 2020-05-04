---------- Studio project ----------

project "Studio"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "studioafx.h"
	pchsource "Core/PCH/studioafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Studio/",
		"$(SolutionDir)Engine/Studio/Core/PCH/",
		"$(SolutionDir)Engine/Source/",
		"$(SolutionDir)Engine/Source/Core/Algorithm",
		"$(SolutionDir)Engine/Source/Core/DataTypes",
		"$(SolutionDir)Engine/Source/Core/Memory",
		"$(SolutionDir)Engine/Source/Core/System",
		"$(SolutionDir)Engine/Source/GameLibraries",
		"$(SolutionDir)Dependencies/Programs/Debugger/src/",
		"$(SolutionDir)Dependencies/Programs/DevCon/src/",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.gli}",
	}

	libdirs {
		"%{IncludeLib.irrKlang}",
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}",
		"%{IncludeLib.Assimp}",
	}

	links { 
		"glew32s",
		"glfw3",
		"opengl32",
		"vulkan-1",
		"irrKlang",
		"assimp",
		"Debugger",
		"Dynamik"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_VULKAN",
			"DMK_USE_VULKAN",
			"GLEW_STATIC",
			"GRAPHICS_API",
			"GLFW_INCLUDE_NONE"
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
