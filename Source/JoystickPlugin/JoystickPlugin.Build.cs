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
				"ApplicationCore",
				"Chaos",
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"InputDevice",
				"PhysicsCore",
				"Projects",
				"Slate",
				"SlateCore",
				"UMG"
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

		// Prefer the symbol-prefixed JoystickSDL3 binaries (built by the build-sdl action);
		// fall back to stock SDL3 names so the plugin still builds against older artifacts.
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var Win64Path = Path.Combine(SdlDirectory, "Win64");
			var LibPath = Path.Combine(Win64Path, "JoystickSDL3.lib");
			var DllPath = Path.Combine(Win64Path, "JoystickSDL3.dll");
			if (!File.Exists(LibPath) || !File.Exists(DllPath))
			{
				LibPath = Path.Combine(Win64Path, "SDL3.lib");
				DllPath = Path.Combine(Win64Path, "SDL3.dll");
			}

			if (!File.Exists(LibPath) || !File.Exists(DllPath))
			{
				throw new BuildException(
					"SDL3 Win64 binaries not found. Expected:\n" +
					$"  {LibPath}\n" +
					$"  {DllPath}");
			}

			PublicAdditionalLibraries.Add(LibPath);

			RuntimeDependencies.Add(DllPath, StagedFileType.NonUFS);

			PublicDelayLoadDLLs.Add(Path.GetFileName(DllPath));
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			var LinuxPath = Path.Combine(SdlDirectory, "Linux");
			var SdlSoPath = Path.Combine(LinuxPath, "libJoystickSDL3.so");
			if (!File.Exists(SdlSoPath))
			{
				SdlSoPath = Path.Combine(LinuxPath, "libSDL3.so.0");
			}

			if (!File.Exists(SdlSoPath))
			{
				throw new BuildException(
					"SDL3 Linux binaries not found. Expected:\n" +
					$"  {SdlSoPath}");
			}

			PublicAdditionalLibraries.Add(SdlSoPath);
			// Stage next to the target binary so the ${ORIGIN} rpath resolves it in packaged builds.
			RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)", Path.GetFileName(SdlSoPath)), SdlSoPath, StagedFileType.NonUFS);
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			var MacPath = Path.Combine(SdlDirectory, "Mac");
			var SdlDylibPath = Path.Combine(MacPath, "libJoystickSDL3.0.dylib");
			if (!File.Exists(SdlDylibPath))
			{
				SdlDylibPath = Path.Combine(MacPath, "libSDL3.0.dylib");
			}

			PublicAdditionalLibraries.Add(SdlDylibPath);

			RuntimeDependencies.Add(SdlDylibPath, StagedFileType.NonUFS);
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
