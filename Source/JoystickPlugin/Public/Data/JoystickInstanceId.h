#pragma once

#include "JoystickInstanceId.generated.h"

USTRUCT(BlueprintType)
struct FJoystickInstanceId
{
	GENERATED_BODY()

	FJoystickInstanceId(): Value(-1)
	{
	}

	FJoystickInstanceId(const int NewValue) : Value(NewValue)
	{
	}

	operator int() const
	{
		return Value;
	}

	bool operator==(const int& Other) const
	{
		return Equals(Other);
	};

	bool operator==(const FJoystickInstanceId& Other) const
	{
		return Equals(Other);
	};

	bool Equals(const FJoystickInstanceId& Other) const
	{
		return Value == Other.Value;
	}

	friend uint32 GetTypeHash(const FJoystickInstanceId& Other)
	{
		return GetTypeHash(Other.Value);
	}

	UPROPERTY(VisibleAnywhere, Category="Device Config", DisplayName="Instance Id")
	int Value;
};
