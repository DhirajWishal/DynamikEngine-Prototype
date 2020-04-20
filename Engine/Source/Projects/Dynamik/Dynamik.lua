---------- Dynamik Engine project description ----------

project "Dynamik"
	kind "SharedLib"
	language "C++"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "dmkafx.h"
	pchsource "../../Runtime/Core/PCH/dmkafx.cpp"

	files {
		"../../Runtime/**.txt",
		"../../Runtime/**.cpp",
		"../../Runtime/**.h",
		"**.lua",
		"**.txt"
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
		"%{IncludeLib.irrKlang}",
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}",
		"%{IncludeLib.Assimp}",
	}

	links { 
		"glfw3dll",
		"opengl32",
		"vulkan-1",
		"irrKlang",
		"assimp",
		"assimp-vc140-mt",
		"Debugger",
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