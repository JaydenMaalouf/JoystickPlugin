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
	UFUNCTION(BlueprintPure, Category = "Joystick|Hat")
	static FVector2D HatDirectionToFVector2D(const EHatDirection Direction);

	static EHatDirection HatValueToDirection(int8 Value);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Hat")
	static FString HatDirectionAsString(EHatDirection Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Key")
	static bool IsJoystickKey(const FKey& Key);
};
