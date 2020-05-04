![Dynamik Engine Logo](https://github.com/DhirajWishal/Dynamik/blob/master/Dependencies/Assets/icons/Samples/DynamikL.png)
# 
Dynamik Engine is a community driven game engine currently at its basic stages. This engine is primarily
designed for 3D FPS games.

The Dynamik Engine is a library which consists a list of independent/ partially independent APIs and Tools which
the user can use to develop their software with. 

The list of APIs:
- Dynamik/ Core:
  - Resource managers.
  - Memory managers.
  - Thread managers.
- Rendering.
- Audio.
- Physics.
- Intellect.
- Destruction.
- Event.
- Networking.
- Validation.
- Game Library

The engine is intended to be compiled to platform specific dynamic link library and will support multiple platforms.
File structures are yet to be designed.

The Studio is an application which uses the Dynamik Engine library extensively. It is a cross-platform tool
which can be used to edit and/ or create objects/ scenes/ levels scripts, etc... The users can also create their
own studio like software using the Dynamik Engine library with the help of the Studio.

The engine provides samples/ templates for users to directly work with. The users are able to chose the sample by 
the Studio and work on it to create their own product. They can use these samples along with our APIs to create
their own samples/ templates. 

The clients code is compiled into either a static link library or dynamic link library depending on the user
preference and will be linked with our executable binary which is platform specific. This way the user can
export their code to other platforms using the Studio and our executable will be able to execute the users 
application. 

The Engine does not handle all the resources provided to it by itself. The users are expected to manage them 
with the of tools that the engine provides. The main execution loop is given to the user to work with which
would enable the user to optimize the code according to their preference. By default there will be at least 3 
threads executed by the engine (Engine Control Thread, Renderer Thread, Utility Thread) which makes a total of
4 threads depending on the number of threads the user creates during runtime. These threads are managed by the 
engine and the user would not have direct control over their execution in runtime except for any commands are
issued.

Further improvements are yet to be made.

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