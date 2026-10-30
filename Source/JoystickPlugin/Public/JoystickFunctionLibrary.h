// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "InputCoreTypes.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Input/HatDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "JoystickFunctionLibrary.generated.h"

struct SDL_GUID;

UCLASS()
class JOYSTICKPLUGIN_API UJoystickFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Joystick|Hat")
	static FVector2D HatDirectionToFVector2D(const EHatDirection Direction);

	static float NormalizeAxisRaw(const int16 Value);

	static EHatDirection HatValueToDirection(const int8 Value);

	// The current time on the same clock as the input data's timestamps: nanoseconds since SDL started.
	static uint64 GetTicksNS();

	// The current time on the same clock as force feedback effect playback: milliseconds since SDL started, the unit SDL's haptic effects are timed in.
	static uint64 GetTicks();

	UFUNCTION(BlueprintPure, Category="Joystick|Hat")
	static FString HatDirectionAsString(EHatDirection Value);

	UFUNCTION(BlueprintPure, Category="Joystick|Key")
	static bool IsJoystickKey(const FKey& Key);

	UFUNCTION(BlueprintPure, Category="Joystick|Instance")
	static bool IsValidInstanceId(const FJoystickInstanceId& InstanceId);

	static FString SanitiseDeviceName(const FString& InDeviceName, const bool AllowSpaces = false);
	static FString SafelyStringify(const wchar_t* Input);
	static FString SafelyStringify(const char* Input);
	static FGuid ConvertSDLGuid(const SDL_GUID& SdlGuid);
};
