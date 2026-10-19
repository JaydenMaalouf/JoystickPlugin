// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Debug/JoystickDebugFontKey.h"

class UCanvas;
class UWorld;

// A debug panel the FJoystickDebugDrawer stacks under the others that are enabled.
class FJoystickDebugOverlay
{
public:
	FJoystickDebugOverlay(const FString& InTitle, const FString& InConsoleCommand, const FString& InEmptyText);
	virtual ~FJoystickDebugOverlay() = default;

	virtual bool IsEnabled() const = 0;

	// Draws the overlay from Y down and returns the Y below it.
	float Draw(UCanvas* Canvas, const UWorld* World, float Y);

protected:
	// Gathers what to draw this frame and returns how many devices were found.
	virtual int32 GatherDevices(const UWorld* World) = 0;

	// Only called when the debug font changes.
	virtual void UpdateFontMetrics(UCanvas* Canvas) = 0;

	// Lays out the columns and returns the panel width.
	virtual float BuildLayout(UCanvas* Canvas) = 0;

	virtual float DrawDevices(UCanvas* Canvas, float Width, float Y) const = 0;

	static void DrawLevelBar(UCanvas* Canvas, float X, float Y, float Level, bool IsClipping);
	static FString FormatLevel(float Level);
	static float GetPanelWidth(float ContentWidth);
	static FLinearColor GetRecessedColor();

	static constexpr float LevelBarWidth = 160.0f;
	static constexpr const TCHAR* LevelSampleText = TEXT("+0.00");

private:
	FJoystickDebugFontKey FontKey;
	FString Title;
	FString ConsoleCommand;
	FString EmptyText;
};
