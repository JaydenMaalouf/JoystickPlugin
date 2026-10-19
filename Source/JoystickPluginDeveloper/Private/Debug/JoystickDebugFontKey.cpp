// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Debug/JoystickDebugFontKey.h"

#include "Engine/Engine.h"
#include "Engine/Font.h"

bool FJoystickDebugFontKey::Update()
{
	const UFont* CurrentFont = GEngine->GetSmallFont();
	if (CurrentFont == Font && CurrentFont->LegacyFontSize == LegacyFontSize)
	{
		return false;
	}

	Font = CurrentFont;
	LegacyFontSize = CurrentFont->LegacyFontSize;
	return true;
}
