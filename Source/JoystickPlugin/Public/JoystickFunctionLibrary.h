// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "InputCoreTypes.h"
#include "Data/Input/HatDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class JOYSTICKPLUGIN_API UJoystickFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Joystick|Hat")
	static FVector2D HatDirectionToFVector2D(const EHatDirection Direction);

	static float NormalizeAxisRaw(const int16 Value);

	static EHatDirection HatValueToDirection(const int8 Value);

	UFUNCTION(BlueprintPure, Category="Joystick|Hat")
	static FString HatDirectionAsString(EHatDirection Value);

	UFUNCTION(BlueprintPure, Category="Joystick|Key")
	static bool IsJoystickKey(const FKey& Key);

	static FString SanitiseDeviceName(const FString& InDeviceName, const bool AllowSpaces = false);
};
