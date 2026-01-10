// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickLedInformation.generated.h"

USTRUCT(BlueprintType)
struct FJoystickLedInformation
{
	GENERATED_BODY()

	FJoystickLedInformation()
		: Mono(false)
		  , RGB(false)
		  , Player(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Led Config")
	bool Mono;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Led Config")
	bool RGB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Led Config")
	bool Player;
};
