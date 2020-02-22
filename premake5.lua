workspace "Dynamik"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

outputDir = "$(Configuration)-$(Platform)"
solutionDir = "$(SolutionDir)"
configurationMacro = "$(Configuration)"
platformMacro = "$(Platform)"
projectNameMacro = "$(ProjectName)"

engineSourcePath = "$(SolutionDir)Engine/Source/"
engineOutputDir = "$(SolutionDir)Builds/Engine/"
engineOutputBinaryDir = "Binaries/"
engineOutputIntermediateDir = "Intermediate/"

IncludeDir = {}
IncludeDir["GLFW"] = "$(SolutionDir)Dependencies/Libraries/External/glfw/include"
IncludeDir["GLEW"] = "$(SolutionDir)Dependencies/GLEW/include"
IncludeDir["glm"] = "$(SolutionDir)Dependencies/Libraries/External/glm"
IncludeDir["stb"] = "$(SolutionDir)Dependencies/Libraries/External/stb"
IncludeDir["tol"] = "$(SolutionDir)Dependencies/Libraries/External/tinyobjloader"
--Include the Vulkan SDK Include path here
IncludeDir["Vulkan"] = "$(SolutionDir)Dependencies/Libraries/External/Vulkan/include"
IncludeDir["boost"] = "E:/Programming/Modules and Runtime/boost_1_70_0"
IncludeDir["gil"] = "$(SolutionDir)Dependencies/Libraries/Local/gil/boost"
IncludeDir["jpeg"] = "$(SolutionDir)Dependencies/Libraries/Local/gil/jpeg-6b"
IncludeDir["irrKlang"] = "$(SolutionDir)/Dependencies/Libraries/Local/irrKlang/include"
IncludeDir["assimp"] = "$(SolutionDir)/Dependencies/Libraries/Local/assimp"

IncludeLib = {}
--Include the GLFW library path here
IncludeLib["GLFW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLFW/lib-vc2019"
IncludeLib["GLEW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLEW/Release/x64"
--Include the Vulkan SDK path here
IncludeLib["Vulkan"] = "$(SolutionDir)Dependencies/Libraries/Binaries/Vulkan"
IncludeLib["irrKlang"] = "$(SolutionDir)Dependencies/Libraries/Binaries/irrKlang"
IncludeLib["boostD"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/debug/address-model-64/link-static/threading-multi"
IncludeLib["boostR"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/release/address-model-64/link-static/threading-multi"

include "Launcher"
include "Application"
include "Engine/Source/Dynamik"
include "Engine/Source/Components/ADGR"
include "Engine/Source/Components/Events"
include "Engine/Source/Components/DevConsole"
include "Engine/Source/Components/Audio"
include "Engine/Source/Components/Platform"
include "Engine/Source/Components/Debugger"
include "Engine/Source/Components/CentralDataHub"
include "Engine/Source/Components/Networking"
include "Engine/Source/Components/Managers"
