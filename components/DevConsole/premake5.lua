---------- Developer Console project ----------

project "DevCon"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "dcnsafx.h"
	pchsource "DevConsole/src/dcnsafx.cpp"

	files {
		"**.cpp",
		"**.h",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)components/DevConsole",
		"$(SolutionDir)components/DevConsole/src",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)components/Debugger/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.Vulkan}"
	}

	links {
		"Dynamik",
		"Debugger"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS"
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
