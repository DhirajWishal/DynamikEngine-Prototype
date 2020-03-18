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
IncludeDir["irrKlang"] = "$(SolutionDir)Dependencies/Libraries/Local/irrKlang/include"
IncludeDir["assimp"] = "$(SolutionDir)Dependencies/Libraries/Local/assimp"

IncludeLib = {}
--Include the GLFW library path here
IncludeLib["GLFW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLFW/lib-vc2019"
IncludeLib["GLEW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLEW/Release/x64"
--Include the Vulkan SDK path here
IncludeLib["Vulkan"] = "$(SolutionDir)Dependencies/Libraries/Binaries/Vulkan"
IncludeLib["irrKlang"] = "$(SolutionDir)Dependencies/Libraries/Binaries/irrKlang"
IncludeLib["boostD"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/debug/address-model-64/link-static/threading-multi"
IncludeLib["boostR"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/release/address-model-64/link-static/threading-multi"

--Dynamik Shared Library Includes
DynamikSharedLibs = {}
DynamikSharedLibs["ADGR"] =				"$(SolutionDir)Engine/Source/Shared/ADGR/src"
DynamikSharedLibs["Audio"] =			"$(SolutionDir)Engine/Source/Shared/Audio/src"
DynamikSharedLibs["Events"] =			"$(SolutionDir)Engine/Source/Shared/Events/src"
DynamikSharedLibs["Intellect"] =		"$(SolutionDir)Engine/Source/Shared/Intellect/src"
DynamikSharedLibs["Managers"] =			"$(SolutionDir)Engine/Source/Shared/Managers/src"
DynamikSharedLibs["Networking"] =		"$(SolutionDir)Engine/Source/Shared/Networking/src"
--Dynamik Shared Library Binaries
DynamikSharedIncludes = {}
DynamikSharedIncludes["ADGR"] =			"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"
DynamikSharedIncludes["Audio"] =		"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"
DynamikSharedIncludes["Events"] =		"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"
DynamikSharedIncludes["Intellect"] =	"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"
DynamikSharedIncludes["Managers"] =		"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"
DynamikSharedIncludes["Networking"] =	"$(SolutionDir)Builds/Engine/$(Configuration)-$(Platform)"

--Dynamik Internal Library Paths
DynamikInternalLibsPath = "$(SolutionDir)Dependencies/Libraries/Internal/CPP"
DynamikInternalBinaries = "$(SolutionDir)Builds/Dependencies/Libraries/Internal/CPP/Binaries"

include "Dependencies/Libraries/Internal/Internal.lua"

include "Dependencies/Programs/Debugger/Debugger.lua"
include "Dependencies/Programs/DevConsole/DevCon.lua"

include "Launcher"

include "Studio/Source/Runtime/Studio.lua"

include "Engine/Source/Runtime/Dynamik.lua"
include "Engine/Application/Runtime/Application.lua"
include "Engine/Source/Shared/ADGR/ADGR.lua"
include "Engine/Source/Shared/Audio/Audio.lua"
include "Engine/Source/Shared/Events/Events.lua"
include "Engine/Source/Shared/Intellect/Intellect.lua"
include "Engine/Source/Shared/Managers/Managers.lua"
include "Engine/Source/Shared/Networking/Networking.lua"
include "Engine/Source/Internal/Platform/Platform.lua"
include "Engine/Source/Internal/Utilities/Utilities.lua"
include "Engine/Source/Internal/CentralDataHub/CentralDataHub.lua"
