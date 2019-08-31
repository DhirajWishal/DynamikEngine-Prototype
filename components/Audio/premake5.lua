---------- Audio project ----------

project "Audio"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	--pchheader "adgrafx.h"
	--pchsource "src/adgrafx.cpp"

	files {
		"**.h",
		"**.hh",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs",
		"$(SolutionDir)components/Audio",
		"$(SolutionDir)components/Audio/src",
		"$(SolutionDir)libs/irrKlang/include",	
		"$(SolutionDir)components/ADGR",
		"$(SolutionDir)components/ADGR/src",
		"$(SolutionDir)components/Events/src",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.irrKlang}"
		}

	libdirs{
		"$(SolutionDir)libs/irrKlang/lib/Winx64-visualStudio"
	}

	links { 
		"Events",
		"irrKlang"
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