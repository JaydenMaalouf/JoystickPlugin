// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickPointOfViewDirection.h"

#include "HatData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FHatData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	EJoystickPointOfViewDirection Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	EJoystickPointOfViewDirection PreviousDirection;
};
