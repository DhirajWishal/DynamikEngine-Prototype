---------- LibWorkbench project description ----------

project "LibWorkbench"
	kind "ConsoleApp"
	cppdialect "C++17"
	language "C++"

	targetdir "$(SolutionDir)Builds/Dependencies/Libraries/Internal/CPP/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Dependencies/Libraries/Internal/CPP/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "wbafx.h"
	pchsource "wbafx.cpp"

	files {
		"**.txt",
		"**.cpp",
		"**.h",
		"**.lua"
	}

	links {
		"MemoryLib",
		"DataTypesLib",
		"SystemLib",
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Internal/CPP",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS"
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
