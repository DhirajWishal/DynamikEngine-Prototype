---------- ADGR project ----------

project "ADGR"
	kind "SharedLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "adgrafx.h"
	pchsource "src/adgrafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua"
	}

	includedirs {
		"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)components/ADGR",
		"$(SolutionDir)components/ADGR/src",
		"$(SolutionDir)components/Events/src",
		"$(SolutionDir)components/Managers/src",
		"$(SolutionDir)components/Platform/src",
		"$(SolutionDir)components/DevConsole/src",
		"$(SolutionDir)components/Debugger/src",
		"$(SolutionDir)components/CentralDataHub/src",
		"$(SolutionDir)components/Networking/src",
		"$(SolutionDir)components/Core/src",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.irrKlang}",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.Vulkan}"
	}

	links { 
		"glfw3dll",
		"glew32s",
		"opengl32",
		"vulkan-1",
		"Events",
		"Platform",
		"Managers",
		"CentralDataHub",
		"Debugger"
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
