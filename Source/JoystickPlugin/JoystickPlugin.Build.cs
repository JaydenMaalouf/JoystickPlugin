using System.IO;
using System.Linq;
using UnrealBuildTool;

public class JoystickPlugin : ModuleRules
{
	public JoystickPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"SlateCore",
				"Slate",
				"ApplicationCore",
				"InputCore",
				"InputDevice",
				"UMG",
				"Projects"
			});

		var Sdl2IncludePath = Path.Combine(EngineDirectory, "Source", "ThirdParty", "SDL2", "SDL-gui-backend", "include");

		PublicIncludePaths.Add(Sdl2IncludePath);

		var SdlDirectory = Path.Combine(PluginDirectory, "ThirdParty", "SDL2");
		var SdlPlatformDirectory = Path.Combine(SdlDirectory, Target.Platform.ToString());

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			RuntimeDependencies.Add(Path.Combine(SdlPlatformDirectory, "SDL2.dll"));
			PublicAdditionalLibraries.Add(Path.Combine(SdlPlatformDirectory, "SDL2.lib"));

			PublicDelayLoadDLLs.Add("SDL2.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			//SDL should be loaded as part of the engine
		}

		var ProfilesDirectory = Path.Combine(PluginDirectory, "Profiles");
		if (Directory.Exists(ProfilesDirectory) && Directory.EnumerateFiles(ProfilesDirectory).Any())
		{
			var ProfileFiles = Path.Combine(ProfilesDirectory, "*.ini");
			RuntimeDependencies.Add(ProfileFiles);
		}

		var GameControllerDbFile = Path.Combine(PluginDirectory, "ThirdParty", "gamecontrollerdb.txt");
		if (File.Exists(GameControllerDbFile))
		{
			RuntimeDependencies.Add(GameControllerDbFile);
		}
	}
}