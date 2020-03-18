---------- Studio project ----------

project "Studio"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir  "$(SolutionDir)Builds/Studio/Binaries"
	objdir "$(SolutionDir)Builds/Studio/Intermediate/$(ProjectName)"

	pchheader "studioafx.h"
	pchsource "src/studioafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Studio/Source/Runtime/src",
		"%{DynamikSharedLibs.ADGR}",
		"%{DynamikSharedLibs.Audio}",
		"%{DynamikSharedLibs.Events}",
		"%{DynamikSharedLibs.Intellect}",
		"%{DynamikSharedLibs.Managers}",
		"%{DynamikSharedLibs.Networking}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}",
		DynamikInternalLibsPath
	}

	libdirs {
		"%{DynamikSharedIncludes.ADGR}",
		"%{DynamikSharedIncludes.Audio}",
		"%{DynamikSharedIncludes.Events}",
		"%{DynamikSharedIncludes.Intellect}",
		"%{DynamikSharedIncludes.Managers}",
		"%{DynamikSharedIncludes.Networking}",

		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}",
		DynamikInternalBinaries
	}

	links { 
		"glfw3dll",
		"glew32s",
		"opengl32",
		"vulkan-1",

		"DataTypesLib",
		"MemoryLib",
		"SystemLib",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL",
			"DMK_USE_VULKAN",
			--"DMK_USE_OPENGL",
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
