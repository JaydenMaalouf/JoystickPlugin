//

namespace UnrealBuildTool.Rules
{
	using System;
	using System.IO;
	using System.Collections.Generic;

	public class JoystickPlugin : ModuleRules
	{

		// UE does not copy third party dlls to the output directory automatically.
		// Link statically so you don't have to do it manually.
		// to be delete private bool LinkThirdPartyStaticallyOnWindows = false;

		//private string ModulePath
		//{
  //          get { return  ModuleDirectory; }
  //      }

  //      private string ThirdPartyPath
		//{
		//	get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
		//}

		//private string BinariesPath
		//{
		//	get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
		//}

		public JoystickPlugin(ReadOnlyTargetRules Target) : base(Target)
		{
            bLegacyPublicIncludePaths = false;
            ShadowVariableWarningLevel = WarningLevel.Error;
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            //DefaultBuildSettings = BuildSettingsVersion.V2;

            PublicDefinitions.Add("SDL_DEPRECATED=1");
            PublicDefinitions.Add("SDL_WITH_EPIC_EXTENSIONS=1");
            PrivatePCHHeaderFile = "Private/JoystickPluginPrivatePCH.h";

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
                    "SDL2_2010"
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

   //         if (Target.Platform == UnrealTargetPlatform.Win64)
			//{
   //             string SDL2Path = ThirdPartyPath + "sdl2_x64-windows/";

   //             string SDL2binPath = SDL2Path + "bin/";
   //             string SDL2LibPath = SDL2Path + "lib/";
   //             string SDL2LibManualPath = SDL2Path + "lib/manual - link/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/SDL2"));                

   //             if (LinkThirdPartyStaticallyOnWindows) {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2-static.lib"));
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibManualPath, "SDL2main.lib"));
   //             } else
   //             {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2binPath, "SDL2.dll"));
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2.lib"));                    
   //             }

   //             PublicAdditionalLibraries.Add("Version.lib");
   //         }

   //         if (Target.Platform == UnrealTargetPlatform.Win32)
   //         {
   //             string SDL2Path = ThirdPartyPath + "sdl2_x86-windows/";
   //             string SDL2LibPath = SDL2Path + "Lib/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/SDL2"));

   //             if (LinkThirdPartyStaticallyOnWindows)
   //             {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2-static.lib"));
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2main.lib"));
   //             }
   //             else
   //             {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2.lib"));
   //             }

   //             PublicAdditionalLibraries.Add("Version.lib");
   //         }
   //         else if (Target.Platform == UnrealTargetPlatform.Mac) // NEVER TESTED
			//{				
   //             string SDL2Path = ThirdPartyPath + "SDL2/SDL/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));
   //             PrivateIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

   //             PublicFrameworks.Add("/Library/Frameworks/SDL2.framework");
			//}
			//else if (Target.Platform == UnrealTargetPlatform.Linux) // NEVER TESTED
   //         {		
			//	string SDL2Path = ThirdPartyPath + "SDL2/SDL/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));
   //             PrivateIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

   //             //AddThirdPartyPrivateStaticDependencies(Target, new string[]{"SDL2"});
   //             AddEngineThirdPartyPrivateStaticDependencies(Target, new string[]{"SDL2"});						
			//}

		}
	}

}
