// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SDL2_VCPKG : ModuleRules
{
	public SDL2_VCPKG(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicDefinitions.Add("SDL_STATIC=0");
        PublicDefinitions.Add("SDL_SHARED=1");
        PublicDefinitions.Add("EPIC_EXTENSIONS=0");

        //string SDL2Path = ThirdPartyPath + "sdl2_x64-windows/";

        string SDL2binPath = Path.Combine(ModuleDirectory, "bin");
		string SDL2LibPath = Path.Combine(ModuleDirectory, "lib");
		string SDL2LibManualPath = Path.Combine(ModuleDirectory, "lib", "manual-link"); 

        string includePath = Path.Combine(ModuleDirectory, "include", "SDL2");
        PublicIncludePaths.AddRange(new string[] { includePath });

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			
			PublicLibraryPaths.Add(SDL2LibPath);
			PublicAdditionalLibraries.Add("SDL2.lib");
			
			PublicLibraryPaths.Add(SDL2LibManualPath);
            PublicAdditionalLibraries.Add("SDL2main.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "Binaries/Win64/"));
			RuntimeDependencies.Add("SDL2.dll");
		}
		
		PublicAdditionalLibraries.Add("Version.lib");
	}
}
