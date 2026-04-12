// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickPowerState.h"

#include "JoystickPowerInformation.generated.h"

USTRUCT(BlueprintType)
struct FJoystickPowerInformation
{
	GENERATED_BODY()

	FJoystickPowerInformation()
		: State(EJoystickPowerState::Unknown)
		  , Level(-1)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Power Config")
	EJoystickPowerState State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Power Config")
	int Level;
};
