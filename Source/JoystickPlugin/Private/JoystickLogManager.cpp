// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickLogManager.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_error.h"

THIRD_PARTY_INCLUDES_END

DEFINE_LOG_CATEGORY(LogJoystickPlugin);

static TSharedPtr<FJoystickLogManager> LogManager;

FJoystickLogManager* FJoystickLogManager::Get()
{
	if (!LogManager.IsValid())
	{
		LogManager = MakeShareable(new FJoystickLogManager());
	}

	return LogManager.Get();
}

void FJoystickLogManager::LogSDLError(const FString& Message)
{
	if (!CanLog())
	{
		return;
	}

	const FString ErrorMessage = FString(SDL_GetError());
	LogError(TEXT("%s: %s"), *Message, *ErrorMessage);
}
