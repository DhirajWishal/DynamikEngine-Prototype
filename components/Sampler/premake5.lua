---------- Sampler project description ----------
project "Sampler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "smpafx.h"
	pchsource "src/smpafx.cpp"

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
		"$(SolutionDir)lib/libs",
		"$(solutionDir)components/Sampler",
		"$(solutionDir)components/Sampler/src",
		"$(solutionDir)components/Events/src",
		"$(solutionDir)components/Platform/src",
		"$(solutionDir)components/Managers/src",
		"$(SolutionDir)libs/glm/**.hpp",
		"$(SolutionDir)libs/glm/**.inl",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tol}",
		"%{IncludeDir.Vulkan}"
	}

	libdirs {
		"$(SolutionDir)libs/GLFW/lib-vc2019",
		"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	}

	links { 
		"Events",
		"Platform",
		"glfw3dll",
		"opengl32",
		"vulkan-1"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"DMK_PLATFORM_WINDOWS",
			"DMK_BUILD_DLL",
			"GLFW_INCLUDE_VULKAN"
		}

	filter "configurations:Debug"
		defines "DMK_DEBUG"
		--runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "DMK_RELEASE"
		--runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "DMK_DISTRIBUTION"
		--runtime "Release"
		optimize "On"