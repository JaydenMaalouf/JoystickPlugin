// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SDL2_208 : ModuleRules
{
	public SDL2_208(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicDefinitions.Add("SDL_STATIC=0");
        PublicDefinitions.Add("SDL_SHARED=1");
        PublicDefinitions.Add("EPIC_EXTENSIONS=0");        

        string includePath = Path.Combine(ModuleDirectory, "include", "SDL2-2.0.8");
        PublicIncludePaths.AddRange(new string[] { includePath });

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "x64", "SDL2.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "x64", "SDL2main.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "x64", "SDL2.dll"));

            // Add the import library
            //PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib", "x64"));
            //PublicAdditionalLibraries.Add("SDL2.lib");
            //PublicAdditionalLibraries.Add("SDL2main.lib");

            // Delay-load the DLL, so we can load it from the right place first
            //PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "bin", "x64"));
			//RuntimeDependencies.Add("SDL2.dll");
		}
		
		PublicAdditionalLibraries.Add("Version.lib");
	}
}
