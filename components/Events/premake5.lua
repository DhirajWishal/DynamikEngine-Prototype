---------- Events project ----------

project "Events"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

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
		"$(SolutionDir)components/Audio/src",
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

	links {
		"Debugger"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
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