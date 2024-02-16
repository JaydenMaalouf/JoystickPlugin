// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickLogManager.h"

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
