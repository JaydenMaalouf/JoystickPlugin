// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickPluginModule.h"

#include "Engine/Engine.h"
#include "Interfaces/IPluginManager.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Misc/Paths.h"
#include "SDLDynamicLoader.h"

#define LOCTEXT_NAMESPACE "JoystickPlugin"

TSharedPtr<IInputDevice> FJoystickPluginModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	JoystickInputDevice = MakeShareable(new FJoystickInputDevice(InMessageHandler));
	if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
	{
		JoystickSubsystem->InitialiseInputDevice(JoystickInputDevice);
	}

	return JoystickInputDevice;
}

void FJoystickPluginModule::StartupModule()
{
	const FString BaseDirectory = IPluginManager::Get().FindPlugin(PluginName)->GetBaseDir();
	PluginDirectory = FPaths::ConvertRelativePathToFull(BaseDirectory);
	PluginSourceDirectory = FPaths::Combine(PluginDirectory, TEXT("Source"));
	PluginThirdPartyDirectory = FPaths::Combine(PluginSourceDirectory, TEXT("ThirdParty"));

#if PLATFORM_WINDOWS
	const FString SdlDir = FPaths::Combine(PluginThirdPartyDirectory, TEXT("SDL3"), TEXT("Win64"));
	const FString SdlDllDir = FPaths::Combine(SdlDir, "SDL3.dll");
	
	FSDLDynamicLoader::Get().Load(SdlDllDir);
#elif PLATFORM_LINUX
	const FString SdlDir = FPaths::Combine(PluginThirdPartyDirectory, TEXT("SDL3"), TEXT("Linux"));
	const FString SdlSoPath = FPaths::Combine(SdlDir, "libSDL3.so");
	
	FSDLDynamicLoader::Get().Load(SdlSoPath);
#endif

	IJoystickPlugin::StartupModule();
}

void FJoystickPluginModule::ShutdownModule()
{
	FSDLDynamicLoader::Get().Unload();

	IJoystickPlugin::ShutdownModule();

	if (JoystickInputDevice.IsValid())
	{
		JoystickInputDevice.Reset();
	}
}

FString FJoystickPluginModule::PluginName = "JoystickPlugin";

FString FJoystickPluginModule::PluginDirectory = "JoystickPlugin";

FString FJoystickPluginModule::PluginSourceDirectory = "JoystickPlugin/Source";

FString FJoystickPluginModule::PluginThirdPartyDirectory = "JoystickPlugin/Source/ThirdParty";

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginModule, JoystickPlugin)
