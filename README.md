![Dynamik Engine Logo](https://github.com/DhirajWishal/Dynamik/blob/master/Dependencies/Assets/icons/Samples/DynamikL.png)
# 
Dynamik Engine is a community driven game engine currently at its basic stages. This engine is primarily
designed for 3D FPS games.

The Dynamik Engine is a library which consists a list of independent/ partially independent APIs and Tools which
the user can use to develop their software with. 

The engine is intended to be compiled to a static link library and will support multiple platforms. File structures
are yet to be designed.

The Studio is an application which uses the Dynamik Engine library extensively. It is a cross-platform tool
which can be used to edit and/ or create objects/ scenes/ levels scripts, etc...

The engine is a standalone component which handles all the provided resources by itself. The client provides the 
engine with the appropriate data using the DMKGameObject as its base class. Game logic is also given to the engine
this exact way.

Game Library components:
- DMKGameObject
  - DMKDebugObject
  - DMKSceneComponent
    - DMKStaticObject
    - DMKSkyBoxObject
    - DMKLightObject
    - DMKMaterialObject

Further improvements are yet to be made.

### First Run:
1. Double click (run/ execute) the GenProject.bat file.
2. Open the Dynamik.sln file in Visual Studio.
3. Unzip the mars.zip or moon.zip in "core assets/" and set the assets path in "Application/paths.txt".
   - Eg: E:/Dynamik/core assets/moon    (single quotes or double quotes are not needed)
4. Select "Application" in the Solution Explorer as the StartUp Project if not selected by default.
5. Run the Build and Run the solution. 

### Developer notice
If anyone is intrested to support this engine, please make sure to send me an email (wishaldhiraj@gmail.com)
and you can be a contributor of this engine.
Primarily there will be 3 branches: master, engine-dev and studio-dev and contributors can contribute to any
of the above branches except for the master branch. Each branch will be merged after a successful release.
Contributors must write a header for each file as defined in the Header Convention.txt file.