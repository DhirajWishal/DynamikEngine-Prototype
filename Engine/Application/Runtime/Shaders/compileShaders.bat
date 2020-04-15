@echo off
for /r %%f in (.) do (
	pushd %%f

	for %%a in (*.vert) do (
		"E:\Programming\SDKs\Vulkan\1.2.131.2\Bin\glslangValidator.exe" -V "%%a" 
	)
	
	for %%a in (*.frag) do (
		"E:\Programming\SDKs\Vulkan\1.2.131.2\Bin\glslangValidator.exe" -V "%%a" 
	)

	popd %%f
)
PAUSE