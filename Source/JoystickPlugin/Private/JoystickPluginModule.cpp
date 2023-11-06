// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickPluginModule.h"
#include "Misc/Paths.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Interfaces/IPluginManager.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "JoystickPlugin"

TSharedPtr<class IInputDevice> FJoystickPluginModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	JoystickInputDevice = MakeShareable(new FJoystickInputDevice(InMessageHandler));
	if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
	{
		JoystickSubsystem->InitialiseInputDevice(JoystickInputDevice);
	}

	return JoystickInputDevice;
}

void FJoystickPluginModule::StartupModule()
{
#if PLATFORM_WINDOWS
	const FString BaseDir = IPluginManager::Get().FindPlugin("JoystickPlugin")->GetBaseDir();
	const FString SDLDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("SDL2"), TEXT("/Win64/"));

	FPlatformProcess::PushDllDirectory(*SDLDir);
	SdlDllHandle = FPlatformProcess::GetDllHandle(*(SDLDir + "SDL2.dll"));
	FPlatformProcess::PopDllDirectory(*SDLDir);
#endif

	IJoystickPlugin::StartupModule();
}

void FJoystickPluginModule::ShutdownModule()
{
#if PLATFORM_WINDOWS
	FPlatformProcess::FreeDllHandle(SdlDllHandle);
#endif

	IJoystickPlugin::ShutdownModule();

	if (JoystickInputDevice.IsValid())
	{
		JoystickInputDevice.Reset();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginModule, JoystickPlugin)
