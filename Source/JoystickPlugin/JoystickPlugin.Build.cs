// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

using System;
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

		var ThirdPartyDirectory = Path.Combine(PluginDirectory, "Source", "ThirdParty");
		var SdlDirectory = Path.Combine(ThirdPartyDirectory, "SDL3");
		var SdlIncludeParentDirectory = Path.Combine(SdlDirectory, "include");
		var SdlIncludeDirectory = Path.Combine(SdlIncludeParentDirectory, "SDL3");

		if (!Directory.Exists(SdlIncludeDirectory))
		{
			throw new BuildException(
				"SDL headers not found. Checked:\n" +
				$"  Plugin: {SdlIncludeDirectory}\n" +
				"Ensure SDL3 is bundled in the plugin Source/ThirdParty/SDL3/include.");
		}

		PublicSystemIncludePaths.Add(SdlIncludeDirectory);
		PublicSystemIncludePaths.Add(SdlIncludeParentDirectory);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var Win64Path = Path.Combine(SdlDirectory, "Win64");
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

		var GameControllerDbFile = Path.Combine(ThirdPartyDirectory, "gamecontrollerdb.txt");
		if (File.Exists(GameControllerDbFile))
		{
			// Add gamecontrollerdb.txt to runtime deps
			RuntimeDependencies.Add(GameControllerDbFile);
			Console.WriteLine("Successfully loaded GameControllerDb");
		}
	}
}