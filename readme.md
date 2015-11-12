UEJoystickPlugin
====================
OK, this is no more an DirectX or Xinput driven plugins. All getting data etc. are convert and changed to SDL2 functions.

Thank you Ikarus76 and samiljan for the good working basis. My implementation is a bit hacky, but I did not have much exercise and UE 4 is not easy.
I was free and did everything so adjusted as I need it for my lab projects. 

Status:
Sam Persson did code review, polishing and add hotplug functionallity. 

Using the Plugins:
Windows:
	Epic did not deliver SDL2 Binary-Files,so in ThirdParty is an Pre-Compiled Binary from SDL2-Website. 
	The Plugin should compile while loading an Project.
	
Linux:
	Before Using/Compiling the Editor you should follow the instructions for compiling SDL2 on the Wiki. 
	In short: Engine/Source/ThirdParty/SDL2/build.sh
	

OLD -----------------------------------------------------------------------------------------------------

An event-driven [DirectInput Joystick](http://msdn.microsoft.com/en-gb/library/windows/desktop/ee418273) plugin with Input Mapping support for the Unreal Engine 4.

Allows for easy binding of all joysticks that support DirectInput API, including steering wheels and gamepads. Main method of interaction is through Unreal Engine's Input Mapping system. Should you require more nuanced scaling or mixing and splitting of inputs you can use the Blueprint Joystick Component and Blueprint Joystick Interface to extend any blueprint to receive the joystick events.

See [unreal plugin thread](https://forums.unrealengine.com/showthread.php?51237-Joystick-Plugin) for downloads and development updates.

##How to install it##

1. Download Plugin
2. Create new or open a project.
3. Browse to your project root (typically found at *Documents/Unreal Projects/{Your Project Root}*)
4. Copy *Plugins* folder into your Project root.
5. Restart the Editor and open your project again. Plugin should be enabled.

##Setup for Use##

<ol>
<li> Input mapping should be automatically enabled, so you can setup your input mapping binds. But you will not receive joystick input until you've added either a JoystickPluginActor to the scene, or a JoystickComponent to any of your blueprints
<li> <b>Option A</b>: JoystickPluginActor for simple input mapping support
<br>2.1. Select Class Viewer
<br><img src="http://i.imgur.com/YEzVwWt.png">
<br>2.2. Find JoystickPluginActor and drag it into the scene. You can confirm it's in the scene by looking at the Scene Outliner.
<br><img src="http://i.imgur.com/trBbvbU.png">
<br>
<li><b>Option B</b>: Joystick Component and JoystickInterface. If you want control over the joystick in blueprint directly for more fine-tuned control (scaling/mixing etc) as well as full button support (up to 128) past the 16 currently supported IM, this is the method to use.
<br>3.1. add a Joystick Component to the relevant blueprint
<br><img src="http://i.imgur.com/D9p2Ehb.png">
<br>3.2. add a Joystick Interface to the same blueprint
<br><img src="http://i.imgur.com/yd5Us2I.png">
<br>3.3. You're done. Right click on the event graph and type 'Joystick' to narrow your events to Joystick related (NB: current version also emits PluggedIn and Unplugged)
<br><img src="http://i.imgur.com/h3cxZ2L.png">
<br>3.4. You can poll by dragging from the component return node, both latest frame and all events emit a JoystickState struct
<br><img src="http://i.imgur.com/AX9lAcn.png">
</ol>

##Todo##
1. Add MFD
2. Force Feedback support. 

If you're good with Windows API, help out!
