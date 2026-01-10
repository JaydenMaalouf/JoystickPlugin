// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

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

		var SdlPath = Path.Combine(PluginDirectory, "Source", "ThirdParty", "SDL3");
		var SdlIncludePath = Path.Combine(SdlPath, "include", "SDL3");
		var SdlIncludePathParentPath = Path.Combine(SdlPath, "include");

		if (!Directory.Exists(SdlIncludePath))
		{
			throw new BuildException(
				"SDL headers not found. Checked:\n" +
				$"  Plugin: {SdlIncludePath}\n" +
				"Ensure SDL3 is bundled in the plugin ThirdParty/SDL3/include.");
		}

		PublicSystemIncludePaths.Add(SdlIncludePath);
		PublicSystemIncludePaths.Add(SdlIncludePathParentPath);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var Win64Path = Path.Combine(SdlPath, "Win64");
			var LibPath = Path.Combine(Win64Path, "SDL3.lib");
			var DllPath = Path.Combine(Win64Path, "SDL3.dll");

			if (!File.Exists(LibPath) || !File.Exists(DllPath))
			{
				throw new BuildException(
					"SDL3 Win64 binaries not found. Expected:\n" +
					$"  {LibPath}\n" +
					$"  {DllPath}");
			}

			PublicAdditionalLibraries.Add(LibPath);

			RuntimeDependencies.Add(DllPath, StagedFileType.NonUFS);

			PublicDelayLoadDLLs.Add("SDL3.dll");
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