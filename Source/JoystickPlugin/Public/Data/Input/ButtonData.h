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
		  , InvertOutput(false)
	{
	}

	bool GetValue() const
	{
		return InvertOutput ? !ButtonState : ButtonState;
	}

	bool GetPreviousValue() const
	{
		return InvertOutput ? !PreviousButtonState : PreviousButtonState;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool InvertOutput;
};
