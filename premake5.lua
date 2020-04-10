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
IncludeDir["gli"] = "$(SolutionDir)Dependencies/Libraries/External/gli/gli"
--Include the Vulkan SDK Include path here
IncludeDir["Vulkan"] = "$(SolutionDir)Dependencies/Libraries/External/Vulkan/include"
IncludeDir["boost"] = "E:/Programming/Modules and Runtime/boost_1_70_0"
IncludeDir["jpeg"] = "$(SolutionDir)Dependencies/Libraries/Local/gil/jpeg-6b"
IncludeDir["irrKlang"] = "$(SolutionDir)Dependencies/Libraries/Local/irrKlang/include"
IncludeDir["assimp"] = "$(SolutionDir)Dependencies/Libraries/Local/assimp"

IncludeLib = {}
--Include the GLFW library path here
IncludeLib["GLFW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLFW/lib-vc2019"
IncludeLib["GLEW"] = "$(SolutionDir)Dependencies/Libraries/Binaries/GLEW/Release/x64"
--Include the Vulkan SDK path here
IncludeLib["Vulkan"] = "$(SolutionDir)Dependencies/Libraries/Binaries/Vulkan"
IncludeLib["DirectX"] = "$(SolutionDir)Dependencies/Libraries/Binaries/DirectX12"
IncludeLib["irrKlang"] = "$(SolutionDir)Dependencies/Libraries/Binaries/irrKlang"
IncludeLib["boostD"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/debug/address-model-64/link-static/threading-multi"
IncludeLib["boostR"] = "E:/Programming/Modules and Runtime/boost_1_70_0/bin/Dependencies/iostreams/build/msvc-14.2/release/address-model-64/link-static/threading-multi"
IncludeLib["Assimp"] = "$(SolutionDir)Dependencies/Libraries/Binaries/Assimp"

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

group "Libraies"	-- Add all the libraries to a virtual folder
group "Libraries/Internal"	-- Add all the project in /Dependencies/Libraries/Internal to a virtual folder
include "Dependencies/Libraries/Internal/Internal.lua"

group ""

group "Tools"	-- Add all tools to a virtual folder
group "Tools/Debug"	-- All all the debugging tools to a virtual folder
include "Dependencies/Programs/Debugger/Debugger.lua"
include "Dependencies/Programs/DevConsole/DevCon.lua"

group ""

group "Launcher"	-- Add the Launcher to a virtual folder
include "Launcher"

group "Studio"	-- Add the Studio to a virtual folder
include "Studio/Source/Runtime/Studio.lua"

group "Engine"	-- Add the Engine to a virtual folder
include "Engine/Source/Runtime/Dynamik.lua"
include "Engine/Application/Runtime/Application.lua"

group "Engine/Shared Libraries"	-- Add all the Engine's shared libraries to a virtual folder
include "Engine/Source/Shared/ADGR/ADGR.lua"
include "Engine/Source/Shared/Audio/Audio.lua"
include "Engine/Source/Shared/DDE/DDE.lua"
include "Engine/Source/Shared/Events/Events.lua"
include "Engine/Source/Shared/Intellect/Intellect.lua"
include "Engine/Source/Shared/Managers/Managers.lua"
include "Engine/Source/Shared/Networking/Networking.lua"

group "Engine/Internal Libraries"	-- Add all the Engine's internal libraries to a virtual folder
include "Engine/Source/Internal/Platform/Platform.lua"
include "Engine/Source/Internal/Utilities/Utilities.lua"
include "Engine/Source/Internal/CentralDataHub/CentralDataHub.lua"

group ""
