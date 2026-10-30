// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/IConsoleManager.h"

class FOutputDevice;
class UJoystickForceFeedbackComponent;
class UWorld;
struct FJoystickHapticInformation;
struct FJoystickInformation;
struct FJoystickInstanceId;

class FJoystickDebugCommands
{
public:
	FJoystickDebugCommands();

private:
	void ListDevices(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar) const;
	void PlayEffectGroup(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar);
	void StopAll(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar);

	void DestroyPlayedComponents();

	static int StopComponents();
	static int StopDevices();

	static void ListDevice(const FJoystickInformation& Info, bool IsConnected, FOutputDevice& Ar);
	static void StopDevice(const FJoystickInformation& Info);

	static FString FormatInputs(const FJoystickInstanceId& InstanceId);
	static FString FormatProfile(const FJoystickInformation& Info);
	static FString FormatHaptic(const FJoystickInformation& Info);
	static FString FormatHapticEffects(const FJoystickHapticInformation& Haptic);
	static FString FormatRumble(const FJoystickInformation& Info);
	static FString FormatSensors(const FJoystickInformation& Info);
	static FString FormatYesNo(bool Value);

	template <typename EnumType>
	static FString FormatEnum(const EnumType Value)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(Value));
	}

	TArray<TWeakObjectPtr<UJoystickForceFeedbackComponent>> PlayedComponents;

	FAutoConsoleCommandWithWorldArgsAndOutputDevice ListCommand;
	FAutoConsoleCommandWithWorldArgsAndOutputDevice PlayCommand;
	FAutoConsoleCommandWithWorldArgsAndOutputDevice StopAllCommand;
};
