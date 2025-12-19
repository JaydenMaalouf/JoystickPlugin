// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ButtonData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FButtonData
{
	GENERATED_BODY()

	FButtonData()
		: ButtonState(false)
		  , PreviousButtonState(false)
	{
	}

	bool GetValue() const
	{
		return ButtonState;
	}

	bool GetPreviousValue() const
	{
		return PreviousButtonState;
	}

	void Update(const bool& InValue)
	{
		PreviousButtonState = ButtonState;
		ButtonState = InValue;
	}

	void Processed()
	{
		PreviousButtonState = ButtonState;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	bool ButtonState;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	bool PreviousButtonState;
};
