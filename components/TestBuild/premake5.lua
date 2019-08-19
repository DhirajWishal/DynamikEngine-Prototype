---------- TestBuild project ----------

project "TestBuild"
	kind "SharedLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "On"

	targetdir ("$(SolutionDir)bin/" .. outputDir .. "/$(ProjectName)")
	objdir ("$(SolutionDir)intDir/" .. outputDir .. "/$(ProjectName)")

	pchheader "tbafx.h"
	pchsource "src/tbafx.cpp"

	files {
		"**.h",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua"
	}

	includedirs {
		--"$(SolutionDir)libs",
		"$(SolutionDir)Dynamik/src",
		"$(SolutionDir)Dynamik/GameLibraries",
		"$(SolutionDir)lib/libs",
		"$(solutionDir)components/TestBuild",
		"$(solutionDir)components/TestBuild/src",
		"$(solutionDir)components/Events/src",
		--"$(SolutionDir)libs/glm/**.hpp",
		--"$(SolutionDir)libs/glm/**.inl",
		--"%{IncludeDir.GLFW}",
		--"%{IncludeDir.glm}",
		--"%{IncludeDir.stb}",
		--"%{IncludeDir.tol}",
		--"%{IncludeDir.Vulkan}"
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