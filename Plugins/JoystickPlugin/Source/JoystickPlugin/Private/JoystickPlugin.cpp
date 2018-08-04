#include <Engine.h>
#include "JoystickPlugin.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#if WITH_EDITOR
	#include "InputSettingsCustomization.h"
#endif

IMPLEMENT_MODULE(FJoystickPlugin, JoystickPlugin)

#define LOCTEXT_NAMESPACE "JoystickPlugin"



bool FJoystickPlugin::SearchForDllPath(FString _searchBase, FString _dllName)
{
	//Search Plugins folder for an instance of Dll.dll, and add to platform search path
	TArray<FString> directoriesToSkip;
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*_searchBase, Visitor);

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString file = TimestampIt.Key();
		const FString filePath = FPaths::GetPath(file);
		const FString fileName = FPaths::GetCleanFilename(file);
		if (fileName.Compare(_dllName) == 0)
		{
			FPlatformProcess::AddDllDirectory(*filePath); // only load dll when needed for use. Broken with 4.11.
			FPlatformProcess::GetDllHandle(*file); // auto-load dll with plugin - needed as 4.11 breaks above line.
			return true;
		}
	}
	return false;
}

void FJoystickPlugin::StartupModule()
{
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("JoystickPlugin")->GetBaseDir();

#if PLATFORM_WINDOWS
	FString dllName = "SDL2.dll";
	if (SearchForDllPath(FPaths::Combine(*BaseDir, TEXT("Binaries/Win64/")), dllName))
	{
		UE_LOG(LogInit, Log, TEXT("SearchForDllPath SDL2 SUCCESSFUL"));
	}
	else
	{
		//Stop loading - plugin required DLL to load successfully
		checkf(false, TEXT("Failed to load SDL2"));
	}
#endif

	IJoystickPlugin::StartupModule();
	JoystickDevice = MakeShareable(new ::FJoystickDevice());
	//return;

#if WITH_EDITOR
	// Replace parts of the input settings widget to make them wide enough to fit long joystick names
	FInputActionMappingCustomizationExtended::Register();
	FInputAxisMappingCustomizationExtended::Register();
#endif
}

#undef LOCTEXT_NAMESPACE
