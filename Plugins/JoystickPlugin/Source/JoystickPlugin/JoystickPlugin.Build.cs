//

namespace UnrealBuildTool.Rules
{
	using System;
	using System.IO;
	using System.Collections.Generic;

	public class JoystickPlugin : ModuleRules
	{
        //// UE does not copy third party dlls to the output directory automatically.
        //// Link statically so you don't have to do it manually.
        //private bool LinkThirdPartyStaticallyOnWindows = false;

        private string ModulePath
        {
            get { return ModuleDirectory; }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
        }

        private string BinariesPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
        }



        public JoystickPlugin(ReadOnlyTargetRules Target) : base(Target)
		{
            //PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicIncludePaths.AddRange(
                new string[] {
                "JoystickPlugin/Public"
                    // ... add public include paths required here ...
                }
                );


            PrivateIncludePaths.AddRange(
                new string[] {
                "JoystickPlugin/Private",
                    // ... add other private include paths required here ...
                }
                );

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "Engine",
                    "InputCore",
                    "Slate",
                    "SlateCore",
                    "EditorStyle",
                    "CoreUObject",
                    "Sockets",
                    "Networking",
                    "RHI",
                    "RenderCore",
                    "Renderer",
                    "ShaderCore",
                    "HeadMountedDisplay",
                    "Slate",
                    "SlateCore",
                    "Projects",
					// ... add other public dependencies that you statically link with here ...
                    "SDL2_VCPKG"
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
   //             string SDL2Path = ThirdPartyPath + "SDL2/SDL/";
   //             string SDL2LibPath = SDL2Path + "Lib/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));
   //             PrivateIncludePaths.Add(Path.Combine(SDL2Path, "include/"));  // Public || Private !? is there documentation ?

   //             if (LinkThirdPartyStaticallyOnWindows) {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2-static.lib"));
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2main.lib"));
   //             } else
   //             {
   //                 PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "SDL2.lib"));                    
   //             }

   //             PublicAdditionalLibraries.Add("Version.lib");
   //         }

   //         if (Target.Platform == UnrealTargetPlatform.Win32)
   //         {
   //             string SDL2Path = ThirdPartyPath + "SDL2/SDL/";
   //             string SDL2LibPath = SDL2Path + "Lib32/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

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
   //         else if (Target.Platform == UnrealTargetPlatform.Mac)
			//{				
   //             string SDL2Path = ThirdPartyPath + "SDL2/SDL/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));
   //             PrivateIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

   //             PublicFrameworks.Add("/Library/Frameworks/SDL2.framework");
			//}
			//else if (Target.Platform == UnrealTargetPlatform.Linux)
			//{		
			//	string SDL2Path = ThirdPartyPath + "SDL2/SDL/";

   //             PublicIncludePaths.Add(Path.Combine(SDL2Path, "include/"));
   //             PrivateIncludePaths.Add(Path.Combine(SDL2Path, "include/"));

   //             //AddThirdPartyPrivateStaticDependencies(Target, new string[]{"SDL2"});
   //             AddEngineThirdPartyPrivateStaticDependencies(Target, new string[]{"SDL2"});						
			//}

		}
	}

}
