// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Debug/JoystickDebugOverlay.h"

#include "Debug/JoystickDebugCanvas.h"
#include "Styling/StyleColors.h"

FJoystickDebugOverlay::FJoystickDebugOverlay(const FString& InTitle, const FString& InConsoleCommand, const FString& InEmptyText)
	: Title(InTitle)
	  , ConsoleCommand(InConsoleCommand)
	  , EmptyText(InEmptyText)
{
}

float FJoystickDebugOverlay::Draw(UCanvas* Canvas, const UWorld* World, float Y)
{
	const int32 NumDevices = GatherDevices(World);
	if (FontKey.Update())
	{
		UpdateFontMetrics(Canvas);
	}

	const float Width = BuildLayout(Canvas);
	Y = FJoystickDebugCanvas::DrawTitleBar(Canvas, Title, ConsoleCommand, Width, Y);

	if (NumDevices == 0)
	{
		return FJoystickDebugCanvas::DrawEmptyBody(Canvas, EmptyText, Width, Y);
	}

	return DrawDevices(Canvas, Width, Y);
}

void FJoystickDebugOverlay::DrawLevelBar(UCanvas* Canvas, const float X, const float Y, const float Level, const bool IsClipping)
{
	constexpr float BarInset = FJoystickDebugCanvas::BarInset;
	const FVector2D Position(X, Y + BarInset);
	const FVector2D Size(LevelBarWidth, FJoystickDebugCanvas::GetRowHeight() - BarInset * 2.0f);
	constexpr float HalfWidth = LevelBarWidth * 0.5f;
	const float CentreX = Position.X + HalfWidth;
	const float Fill = FMath::Clamp(Level, -1.0f, 1.0f) * HalfWidth;

	FJoystickDebugCanvas::DrawTile(Canvas, Position, Size, GetRecessedColor());
	FJoystickDebugCanvas::DrawTile(Canvas, FVector2D(CentreX + FMath::Min(Fill, 0.0f), Position.Y), FVector2D(FMath::Abs(Fill), Size.Y), FJoystickDebugCanvas::GetLevelColor(Level, IsClipping));
	FJoystickDebugCanvas::DrawTile(Canvas, FVector2D(CentreX, Position.Y), FVector2D(1.0f, Size.Y), FJoystickDebugCanvas::ZeroLineColor);
}

FString FJoystickDebugOverlay::FormatLevel(const float Level)
{
	return FString::Printf(TEXT("%+.2f"), Level);
}

float FJoystickDebugOverlay::GetPanelWidth(const float ContentWidth)
{
	return ContentWidth + FJoystickDebugCanvas::PanelPadding * 2.0f;
}

FLinearColor FJoystickDebugOverlay::GetRecessedColor()
{
	return FJoystickDebugCanvas::GetThemeColor(FStyleColors::Recessed, 1.0f);
}
