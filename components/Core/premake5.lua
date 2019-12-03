---------- Core project ----------

project "Core"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/Application")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "tbafx.h"
	pchsource "src/tbafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"src",
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
		"%{IncludeDir.Glad}",
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

	--libdirs {
	--	"$(SolutionDir)libs/GLFW/lib-vc2019",
	--	"E:/Programming/Codes/Game Development/Libraries/VulkanSDK/1.1.108.0/Lib"
	--}

	--links { 
	--	"glfw3dll",
	--	"opengl32",
	--	"vulkan-1",
	--	"Events"
	--}
	links{
		"Debugger"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"TB_PLATFORM_WINDOWS",
			"TB_BUILD_DLL",
		}

	filter "configurations:Debug"
		defines "TB_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "TB_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "TB_DISTRIBUTION"
		runtime "Release"
		optimize "On"