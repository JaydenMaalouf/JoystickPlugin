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
Compile SDL2 with vc2015 is a painful thing. Compiling as Dynamic and Static will do. But i am not able get the static version linked to my plugin for now. So, for now i am using an dynamic linked version.

0) Clone the repository branch 4.10 for UE4 version 4.10

Next steps only nessessary if the binary versions in repo are older. I put compiled versions for 4.10.1 in the repository.

--------------------------------------------------------------------------------------------------
a) Go to Plugins/JoystickPlugin/ThirdParty/SDL2

b) At first Time run the setup.bat. This will download the lastest hg-branched source from sdl.

c) call the build.bat for the latest version or the build203 for version 2.0.3 of SDL2. This batch-files will copy the binaries to the bin directory of the plugin.

d) If you have installed the VS2013/2015 and DirectX SDK Jun2010 and the Windows 8.1/10 SDK and the Mercurial programm (SDL repo), you should get SDL2-files in the SDL????/Lib directory. 

Now you could delete the build directory

--------------------------------------------------------------------------------------------------

1) You should have an UE4-Project (your project) with C++ Sources. No C++ Sources ? than: Create a Dummy-Class. This will create an VisualStudio Project-File (to recreate: RMB - Generate VisualStudio...).
Once you have create an Class the VS Editor comes up, the Editor initiates compiling in background. Now you can close VS and UE4 Editor.

2) Copy the Plugins-Directory you checked out. (Branch 4.10). Recreate the VS-Project files: RMB - Generate VisualStudio...

3) Open the UE4-Project and be sure you have activated the Joystick-Plugin.

Now you can map Inputs to Joystick devices. (Engine-Input)

Here https://w-hs.sciebo.de/index.php/s/148QVopCDdHwhLQ is a Test-Project ThirdPerson-Template.
Here is an minimum demo project https://w-hs.sciebo.de/index.php/s/qajqJPsk1JGhFFM


-----------------------------------------------------------------------------------------------------------
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
