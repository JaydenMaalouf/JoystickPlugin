// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "BallData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FBallData
{
	GENERATED_BODY()

	FBallData()
		: Direction(FVector2D::ZeroVector)
		  , PreviousDirection(FVector2D::ZeroVector)
	{
	}

	FVector2D GetValue() const
	{
		return Direction;
	}

	FVector2D GetPreviousValue() const
	{
		return PreviousDirection;
	}

	void Update(const FVector2D& InValue)
	{
		PreviousDirection = Direction;
		Direction = InValue;
	}

	void Processed()
	{
		PreviousDirection = Direction;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Ball")
	FVector2D Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Ball")
	FVector2D PreviousDirection;
};
