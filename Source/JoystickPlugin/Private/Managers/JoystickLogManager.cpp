// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickLogManager.h"

#include "SDLDynamicLoader.h"

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

void FJoystickLogManager::LogSDLError(const FStringView Message)
{
	if (!CanLog())
	{
		return;
	}

	const FString ErrorMessage = FString(SDL_GetError());
	if (ErrorMessage.IsEmpty())
	{
		LogError(TEXT("%.*s: Unknown error occurred"), Message.Len(), Message.GetData());
		return;
	}

	LogError(TEXT("%.*s: %s"), Message.Len(), Message.GetData(), *ErrorMessage);
}
