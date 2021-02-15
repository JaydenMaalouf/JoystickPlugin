// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SDL2 : ModuleRules
{
	public string GetUProjectPath()
	{
		return Path.Combine(ModuleDirectory, "../../../../..");
	}

	private string CopyToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
	{
		string BinariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
		string Filename = Path.GetFileName(Filepath);

		//convert relative path 
		string FullBinariesDir = Path.GetFullPath(BinariesDir);

		if (!Directory.Exists(FullBinariesDir))
		{
			Directory.CreateDirectory(FullBinariesDir);
		}

		string FullExistingPath = Path.Combine(FullBinariesDir, Filename);
		bool ValidFile = false;

		//File exists, check if they're the same
		if (File.Exists(FullExistingPath))
		{
			ValidFile = true;
		}

		//No valid existing file found, copy new dll
		if (!ValidFile)
		{
			File.Copy(Filepath, Path.Combine(FullBinariesDir, Filename), true);
		}
		return FullExistingPath;
	}
	
	public SDL2(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicDefinitions.Add("SDL_STATIC=1");
        PublicDefinitions.Add("SDL_SHARED=0");
        PublicDefinitions.Add("EPIC_EXTENSIONS=0");
        PublicDefinitions.Add("SDL_DEPRECATED=1");
        PublicDefinitions.Add("SDL_WITH_EPIC_EXTENSIONS=1");
        PublicDefinitions.Add("WIN32");

        string includePath = Path.Combine(ModuleDirectory, "include");
        PublicIncludePaths.AddRange(new string[] { includePath });

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "SDL2.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "SDL2main.lib"));
            PublicDelayLoadDLLs.Add("SDL2.dll");            
        }
        
		string pluginDLLPath = Path.Combine(ModuleDirectory, "lib", "SDL2.dll");
		string binariesPath = CopyToProjectBinaries(pluginDLLPath, Target);
		System.Console.WriteLine("Using SDL2 DLL: " + binariesPath);
		RuntimeDependencies.Add(binariesPath);   
    }
}
