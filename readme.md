UEJoystickPlugin
====================
OK, this is no more an DirectX or Xinput driven plugins. All getting data etc. are convert and changed to SDL2 functions.

Thank you Ikarus76 and samiljan for the good working basis. My implementation is a bit hacky, but I did not have much exercise and UE 4 is not easy.
I was free and did everything so adjusted as I need it for my lab projects. 

Status:
Sam Persson did code review, polishing and add hotplug functionallity. 

Links for Software needed to compile the plugin:

Windows:

Microsoft DirectX SDK Jun2010 - https://www.microsoft.com/en-us/download/details.aspx?id=6812
Windows 8.1 SDK - https://msdn.microsoft.com/de-de/windows/desktop/bg162891.aspx#


Howto use it

Windows:
Compile SDL2 as static library from source:
1) Go to Plugins/JoystickPlugin/ThirdParty/SDL2

2) call the build.bat for the latest version or the build203 for version 2.0.3 of SDL2.

3) If yoou have installed the VS2013/2015 and DirectX SDK Jun2010 and the Windows 8.1 SDK, at least, you should get an SDL2-static.lib in the SDL2-xxxx/Lib directory.

4) Now you should have an UE4-Project with C++ Sources. At least a Dummy-Class, you need to create an VisualStudio Project-File create from the UE-Project-File (RMB - Generate VisualStudio...).

5) Now open the VS-Solution and compile all.

6) Open the UE4-Project and be sure you have activated the Joystick-Plugin.


Linux (Ubuntu 14.04 LTS): (todo tests on fesh install systems (maybe i forgott something))

Compile SDL2 as static library from source:

1) Go to Engine/Source/ThirdParty/SDL2

2) call build.sh

3) ready with SDL2

4) Copy (or Link <- Symlinks seems not to work?!) the JoystickPlugin into /Engine/Plugins (result -> UnrealEngine/Engine/Plugins/JoystickPlugin)

5) ./GenerateProjectFiles.sh to create the makefile which compiles the joystickplugins with the whole engine

6) compile

7) start editor an enable JoystickPlugin

x) If you have an compiled version of the plugins you can copy/move the joystickplugin-directory to the projects-plugins-directory you want to use it.

Note:
Linux Graphics: On my NVidia GTX750TI i need -opengl4 parameter for the editor.
