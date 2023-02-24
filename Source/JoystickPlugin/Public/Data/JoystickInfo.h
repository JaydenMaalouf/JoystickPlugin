// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickInfo.generated.h"

enum class EInputType : uint8;

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInfo
{
	GENERATED_BODY()

	FJoystickInfo()
		: Player(-1)
		  , DeviceId(-1)
		  , IsGamepad(false)
		  , HasRumble(false)
		  , Connected(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	int Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	int DeviceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	bool IsGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	bool HasRumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	FGuid ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	FString ProductName;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	bool Connected;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
	TArray<EInputType> InputType;
};
