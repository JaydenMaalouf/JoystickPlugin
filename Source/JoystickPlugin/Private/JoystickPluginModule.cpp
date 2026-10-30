// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickPluginModule.h"

#include "Engine/Engine.h"
#include "Interfaces/IPluginManager.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Misc/Paths.h"

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

	// Prefer the symbol-prefixed JoystickSDL3 binary, falling back to stock SDL3 names. Linux links it directly, so needs no manual load.
#if PLATFORM_WINDOWS
	const FString SdlDir = FPaths::Combine(PluginThirdPartyDirectory, TEXT("SDL3"), TEXT("Win64"));
	FString SdlDllPath = FPaths::Combine(SdlDir, TEXT("JoystickSDL3.dll"));
	if (!FPaths::FileExists(SdlDllPath))
	{
		SdlDllPath = FPaths::Combine(SdlDir, TEXT("SDL3.dll"));
	}

	FPlatformProcess::PushDllDirectory(*SdlDir);
	SdlDllHandle = FPlatformProcess::GetDllHandle(*SdlDllPath);
	FPlatformProcess::PopDllDirectory(*SdlDir);
#elif PLATFORM_MAC
	const FString SdlDir = FPaths::Combine(PluginThirdPartyDirectory, TEXT("SDL3"), TEXT("Mac"));
	FString SdlDylibPath = FPaths::Combine(SdlDir, TEXT("libJoystickSDL3.0.dylib"));
	if (!FPaths::FileExists(SdlDylibPath))
	{
		SdlDylibPath = FPaths::Combine(SdlDir, TEXT("libSDL3.0.dylib"));
	}

	SdlDllHandle = FPlatformProcess::GetDllHandle(*SdlDylibPath);
#endif

	IJoystickPlugin::StartupModule();
}

void FJoystickPluginModule::ShutdownModule()
{
#if PLATFORM_WINDOWS || PLATFORM_MAC
	FPlatformProcess::FreeDllHandle(SdlDllHandle);
#endif

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
