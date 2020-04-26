---------- DResValidator project description ----------

project "DResValidator"
	kind "ConsoleApp"
	cppdialect "C++17"
	language "C++"

	targetdir "$(SolutionDir)Builds/Tools/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Tools/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	files {
		"**.txt",
		"**.cpp",
		"**.h",
		"Core/**.cpp",
		"Core/**.h",
		"../Samples/**.dres",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Engine/Source/Runtime/Core/Algorithm",
		"$(SolutionDir)Engine/Source/Runtime/Core/DataTypes",
		"$(SolutionDir)Engine/Source/Runtime/Core/Math",
		"$(SolutionDir)Engine/Source/Runtime/Core/Memory",
		"$(SolutionDir)Engine/Source/Runtime/Core/System",
		"$(SolutionDir)Dependencies/Programs/Debugger/src/",
		"$(SolutionDir)Dependencies/Programs/DevCon/src/",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
	}

	links {
		"Debugger"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		--buildoptions "/MTd"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		--buildoptions "/MT"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		--buildoptions "/MT"
		optimize "On"