![Dynamik Engine Logo](https://github.com/DhirajWishal/Dynamik/blob/master/Dependencies/Assets/icons/Samples/DynamikL.png)
# 
Dynamik Engine is a community driven game engine currently at its basic stages. This engine is primarily
designed for 3D FPS games.

The engine is still basic and could be subjected for some major architectural changes.
Currently following the rule "Every component is a Game Object".

### First Run:
1. Double click (run/ execute) the GenProject.bat file.
2. Open the Dynamik.sln file in Visual Studio.
3. Unzip the mars.zip or moon.zip in "core assets/" and set the assets path in "Application/paths.txt".
   - Eg: E:/Dynamik/core assets/moon    (single quotes or double quotes are not needed)
4. Select "Application" in the Solution Explorer as the StartUp Project if not selected by default.
5. Run the Build and Run the solution. 

## TODO List:
- [ ] Control Panel Window (Editor).
- [ ] Studio window manager.
- [ ] Level editor.
- [ ] Fully functional asset loader.
- [ ] Fully functional animation handler (Vulkan).
- [ ] Vulkan push constant standardization.
- [ ] Physics engine integration.
- [ ] Optimized event system.
- [ ] Render component standardization.
- [ ] Render context and multiple render instances implementation.
- [ ] Particle system implementation.
- [ ] Proper audio engine API.
- [ ] Dynamik Resource file (.dres) tools.
- [ ] Dynamik Script file (.dmk) tools.
- [ ] Engine documentation.

### Developer notice
If anyone is intrested to support this engine, please make sure to send me an email (wishaldhiraj@gmail.com)
and you can be a contributor of this engine.
Primarily there will be 3 branches: master, engine-dev and studio-dev and contributors can contribute to any
of the above branches except for the master branch. Each branch will be merged after a successful release.
Contributors must write a header for each file as defined in the Header Convention.txt file.

#### Folder content
Dependencies: This contains all the dependencies (libraries, binaries, assets) the engine needs.
Documentation: The engine documentation.
Engine: Contains all the engine source code.