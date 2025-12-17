// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "InputCoreTypes.h"
#include "KeyPair.generated.h"

USTRUCT()
struct FKeyPair
{
	GENERATED_BODY()

	FKey X;
	FKey Y;

	FKey& operator[](const int Index)
	{
		check(Index >= 0 && Index < 2);
		return Index == 0 ? X : Y;
	}

	const FKey& operator[](const int Index) const
	{
		check(Index >= 0 && Index < 2);
		return Index == 0 ? X : Y;
	}

	bool IsValid() const
	{
		return X.IsValid() && Y.IsValid();
	}
};
