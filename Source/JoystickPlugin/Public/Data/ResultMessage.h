// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FInternalResultMessage
{
	explicit FInternalResultMessage(const bool bInSuccess)
	{
		bSuccess = bInSuccess;
	}

	FInternalResultMessage(const bool bInSuccess, const FString& InErrorMessage)
	{
		bSuccess = bInSuccess;
		ErrorMessage = InErrorMessage;
	}

	bool bSuccess = false;
	FString ErrorMessage;
};
