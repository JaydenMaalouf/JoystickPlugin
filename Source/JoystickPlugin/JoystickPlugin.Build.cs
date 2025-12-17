using System.IO;
using System.Linq;
using UnrealBuildTool;

public class JoystickPlugin : ModuleRules
{
	public JoystickPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			[
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
			]);
		
		var PluginSdlPath = Path.Combine(PluginDirectory, "ThirdParty", "SDL2");

		var PluginIncludePath = Path.Combine(PluginSdlPath, "include");
		var EngineIncludePath = Path.Combine(EngineDirectory, "Source", "ThirdParty", "SDL2", "SDL-gui-backend", "include");

		var SdlIncludeDir =
			Directory.Exists(EngineIncludePath) ? EngineIncludePath :
			Directory.Exists(PluginIncludePath) ? PluginIncludePath :
			null;

		if (SdlIncludeDir == null)
		{
			throw new BuildException(
				"SDL2 headers not found. Checked:\n" +
				$"  Plugin: {PluginIncludePath}\n" +
				$"  Engine: {EngineIncludePath}\n" +
				"Ensure SDL2 is bundled in the plugin ThirdParty/SDL2/include or available in the Engine third-party path.");
		}

		PublicSystemIncludePaths.Add(SdlIncludeDir);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var Win64Path = Path.Combine(PluginSdlPath, "Win64");
			var LibPath = Path.Combine(Win64Path, "SDL2.lib");
			var DllPath = Path.Combine(Win64Path, "SDL2.dll");

			if (!File.Exists(LibPath) || !File.Exists(DllPath))
			{
				throw new BuildException(
					"SDL2 Win64 binaries not found. Expected:\n" +
					$"  {LibPath}\n" +
					$"  {DllPath}");
			}

			PublicAdditionalLibraries.Add(LibPath);

			RuntimeDependencies.Add(DllPath, StagedFileType.NonUFS);

			PublicDelayLoadDLLs.Add("SDL2.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// SDL should be loaded as part of the engine
		}

		var ProfilesDirectory = Path.Combine(PluginDirectory, "Profiles");
		if (Directory.Exists(ProfilesDirectory) && Directory.EnumerateFiles(ProfilesDirectory).Any())
		{
			// Add all profiles to runtime deps
			var ProfileFiles = Path.Combine(ProfilesDirectory, "*.ini");
			RuntimeDependencies.Add(ProfileFiles);
		}

		var GameControllerDbFile = Path.Combine(PluginDirectory, "ThirdParty", "gamecontrollerdb.txt");
		if (File.Exists(GameControllerDbFile))
		{
			// Add gamecontrollerdb.txt to runtime deps
			RuntimeDependencies.Add(GameControllerDbFile);
		}
	}
}