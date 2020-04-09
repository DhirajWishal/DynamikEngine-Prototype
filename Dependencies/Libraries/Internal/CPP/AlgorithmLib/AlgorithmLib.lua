---------- AlgorithmLib project description ----------

project "AlgorithmLib"
	kind "StaticLib"
	cppdialect "C++17"
	language "C++"
	staticruntime "On"

	targetdir "$(SolutionDir)Builds/Dependencies/Libraries/Internal/CPP/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Dependencies/Libraries/Internal/CPP/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	files {
		"**.txt",
		"**.cpp",
		"**.h",
		"Private/**.cpp",
		"Public/**.h",
		"**.lua"
	}

	links {
		"MemoryLib",
		"DataTypesLib",
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Internal/CPP",
		"%{IncludeDir.gli}"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_LIB",
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
