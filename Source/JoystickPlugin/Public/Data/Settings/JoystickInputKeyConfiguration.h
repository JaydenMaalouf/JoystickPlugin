// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Types/SlateEnums.h"

#include "JoystickInputKeyConfiguration.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputKeyConfiguration
{
	GENERATED_BODY()

	FJoystickInputKeyConfiguration() :
		Direction(EUINavigation::Invalid),
		Action(EUINavigationAction::Invalid)
	{
	}

	UPROPERTY(EditAnywhere, Category="Key Config|UI Navigation", meta=(ToolTip="Adds Widget navigation direction (ie. Up, Down, Left, Right) to this specific key.", ConfigRestartRequired=true))
	EUINavigation Direction;

	UPROPERTY(EditAnywhere, Category="Key Config|UI Navigation", meta=(ToolTip="Adds Widget navigation action (ie. Accept, Back) to this specific key. Only works for UE5.4 onwards", ConfigRestartRequired=true))
	EUINavigationAction Action;
};
