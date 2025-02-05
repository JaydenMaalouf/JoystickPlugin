// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/Input/HatDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class JOYSTICKPLUGIN_API UJoystickFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
	static FVector2D HatDirectionToFVector2D(const EHatDirection Direction);

	static EHatDirection HatValueToDirection(int8 Value);

	UFUNCTION(BlueprintCallable)
	static FString HatDirectionAsString(EHatDirection Value);
};
