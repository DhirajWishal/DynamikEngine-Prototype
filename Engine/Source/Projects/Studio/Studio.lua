---------- Studio project ----------

project "Studio"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir  "$(SolutionDir)Builds/Studio/Binaries"
	objdir "$(SolutionDir)Builds/Studio/Intermediate/$(ProjectName)"

	pchheader "studioafx.h"
	pchsource "../../Studio/Core/PCH/studioafx.cpp"

	files {
		"../../Studio/**.h",
		"../../Studio/**.cpp",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Source/Runtime/",
		"$(SolutionDir)Engine/Source/Runtime/Core/PCH/",
		"$(SolutionDir)Engine/Source/Runtime/Core/Algorithm",
		"$(SolutionDir)Engine/Source/Runtime/Core/DataTypes",
		"$(SolutionDir)Engine/Source/Runtime/Core/Memory",
		"$(SolutionDir)Engine/Source/Runtime/Core/System",
		"$(SolutionDir)Engine/Source/Runtime/GameLibraries",
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
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}",
	}

	links { 
		"glfw3dll",
		"glew32s",
		"opengl32",
		"vulkan-1",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_USE_VULKAN",
			"GLEW_STATIC",
			"GLFW_INCLUDE_VULKAN"
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
