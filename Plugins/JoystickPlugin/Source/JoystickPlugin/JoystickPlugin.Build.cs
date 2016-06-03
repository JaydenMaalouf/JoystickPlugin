// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

//Running D:/UE4/UnrealEngineCG221_RELEASE/Engine/Binaries/DotNET/UnrealBuildTool.exe  -projectfiles -project="C:/Users/thoma/OneDrive/Dokumente/Unreal Projects/VehicleJoystick/VehicleJoystick.uproject" -game -engine -progress
//Discovering modules, targets and source code for project...
//Messages while compiling C:\Users\thoma\OneDrive\Dokumente\Unreal Projects\VehicleJoystick\Intermediate\Build\BuildRules\VehicleJoystickModuleRules.dll:
//c:\Users\thoma\OneDrive\Dokumente\Unreal Projects\VehicleJoystick\Plugins\JoystickPlugin\Source\JoystickPlugin\JoystickPlugin.Build.cs(18,10) : error CS0201: Nur assignment-, call-, increment-, decrement-, await- und 'new object'-Ausdr?cke k?nnen als Anweisung verwendet werden.
//c:\Users\thoma\OneDrive\Dokumente\Unreal Projects\VehicleJoystick\Plugins\JoystickPlugin\Source\JoystickPlugin\JoystickPlugin.Build.cs(114,5) : warning CS0618: 'UnrealBuildTool.ModuleRules.AddThirdPartyPrivateStaticDependencies(UnrealBuildTool.TargetInfo, params string[])' ist veraltet: 'Use AddEngineThirdPartyPrivateStaticDependencies to add dependencies on ThirdParty modules within the Engine Directory'
//UnrealBuildTool Exception: ERROR: UnrealBuildTool encountered an error while compiling source files

namespace UnrealBuildTool.Rules
{
	using System;
	using System.IO;
	using System.Collections.Generic;

	public class JoystickPlugin : ModuleRules
	{
		// UE does not copy third party dlls to the output directory automatically.
		// Link statically so you don't have to do it manually.
		private bool LinkThirdPartyStaticallyOnWindows = false;

        // tsky GetModuleFilename is obsolete --> RulesCompiler.ModuleDirectory
		private string ModulePath
		{
            get { return  ModuleDirectory; }
        }

        private string ThirdPartyPath
		{
			get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
		}

		private string BinariesPath
		{
			get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
		}

		public virtual void SetupBinaries(
			TargetInfo Target,
			ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
			ref List<string> OutExtraModuleNames
			)
		{
			//OutBuildBinaryConfigurations.Add(
				//new UEBuildBinaryConfiguration(UEBuildBinaryType.DynamicLinkLibrary, InTargetName: "SDL2.dll"));
		}

		public JoystickPlugin(TargetInfo Target)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"InputCore",
					"Slate",
					"SlateCore",
					// ... add other public dependencies that you statically link with here ...
				});

			PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
					"InputDevice",
				});

			if (Target.Type == TargetRules.TargetType.Editor)
			{
				PrivateIncludePathModuleNames.AddRange(
					new string[]
					{
						"PropertyEditor",
						"ActorPickerMode",
						"DetailCustomizations",
					});

				PrivateDependencyModuleNames.AddRange(
					new string[]
					{
						"PropertyEditor",
						"DetailCustomizations",
						// ... add private dependencies that you statically link with here ...
					});
			}

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				});			

			if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
			{
                string SDL2Path = ThirdPartyPath + "SDL2/SDL/";
                string SDL2LibPath = SDL2Path + "Lib/";

                PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

                if (LinkThirdPartyStaticallyOnWindows) {
                    PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2-static.lib"));
                    PublicAdditionalLibraries.Add("Version.lib");

                } else
                {
                    PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2.lib"));
                    PublicAdditionalLibraries.Add("Version.lib");
                }
                //PublicAdditionalLibraries.Add("msvcrt.lib");
                //PublicAdditionalLibraries.Add("ucrt.lib");
                //PublicAdditionalLibraries.Add("vcruntime.lib");

            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				PublicFrameworks.Add("/Library/Frameworks/SDL2.framework");
			}
			else if (Target.Platform == UnrealTargetPlatform.Linux)
			{			
				AddThirdPartyPrivateStaticDependencies(Target, "SDL2");
			}

		}
	}

}
