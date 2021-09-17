#pragma once

#include "Data/JoystickPOVDirection.h"

#include "Containers/Array.h"

#include "JoystickState.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickState
{
	GENERATED_USTRUCT_BODY()

	explicit FJoystickState(int32 DeviceId = -1)
	: DeviceId(DeviceId)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
	int32 DeviceId;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
	TArray<float> Axes;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
	TArray<bool> Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
	TArray<EJoystickPOVDirection> Hats;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
	TArray<FVector2D> Balls;
};