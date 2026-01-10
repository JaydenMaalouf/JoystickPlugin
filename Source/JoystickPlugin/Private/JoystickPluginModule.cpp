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
	PluginDirectory = IPluginManager::Get().FindPlugin(PluginName)->GetBaseDir();
#if PLATFORM_WINDOWS
	const FString SdlDir = FPaths::Combine(*PluginDirectory, TEXT("ThirdParty"), TEXT("SDL2"), TEXT("Win64"));

	FPlatformProcess::PushDllDirectory(*SdlDir);
	const FString SdlDllDir = FPaths::Combine(SdlDir, "SDL2.dll");
	SdlDllHandle = FPlatformProcess::GetDllHandle(*SdlDllDir);
	FPlatformProcess::PopDllDirectory(*SdlDir);
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

FString FJoystickPluginModule::PluginName = "JoystickPlugin";

FString FJoystickPluginModule::PluginDirectory = "JoystickPlugin";

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginModule, JoystickPlugin)
