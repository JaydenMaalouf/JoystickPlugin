// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Debug/JoystickDebugCanvas.h"

#include "CanvasItem.h"
#include "Data/JoystickInformation.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "JoystickSubsystem.h"
#include "Styling/StyleColors.h"

float FJoystickDebugCanvas::DrawTitleBar(UCanvas* Canvas, const FString& Title, const FString& Hint, const float Width, const float Y)
{
	return DrawBar(Canvas, Title, Hint, FStyleColors::Title, Width, Y);
}

float FJoystickDebugCanvas::DrawHeader(UCanvas* Canvas, const FString& Title, const FString& Stats, const float Width, const float Y)
{
	return DrawBar(Canvas, Title, Stats, FStyleColors::Header, Width, Y);
}

float FJoystickDebugCanvas::DrawSectionBody(UCanvas* Canvas, const int32 NumRows, const float Width, const float Y, const float ExtraHeight)
{
	const float Height = NumRows * GetRowHeight() + PanelPadding * 2.0f + ExtraHeight;
	DrawPanelTile(Canvas, FVector2D(OriginX, Y), FVector2D(Width, Height), GetThemeColor(FStyleColors::Panel));
	return Y + PanelPadding;
}

float FJoystickDebugCanvas::DrawEmptyBody(UCanvas* Canvas, const FString& Text, const float Width, const float Y)
{
	const float RowY = DrawSectionBody(Canvas, 1, Width, Y);
	DrawRowText(Canvas, Text, ContentX, RowY, GetTextColor(false));
	return RowY + GetRowHeight() + PanelPadding;
}

void FJoystickDebugCanvas::DrawPanelTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color)
{
	if (IsBackgroundEnabled() == false)
	{
		return;
	}

	DrawTile(Canvas, Position, Size, Color);
}

void FJoystickDebugCanvas::DrawTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color)
{
	FCanvasTileItem Tile(Position, Size, Color);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}

void FJoystickDebugCanvas::DrawRowText(UCanvas* Canvas, const FString& Text, const float X, const float Y, const FLinearColor& Color)
{
	FFontRenderInfo RenderInfo;
	RenderInfo.bEnableShadow = IsBackgroundEnabled() == false;

	Canvas->SetDrawColor(Color.ToFColor(true));
	Canvas->DrawText(GEngine->GetSmallFont(), Text, X, Y + RowPadding * 0.5f, 1.0f, 1.0f, RenderInfo);
}

void FJoystickDebugCanvas::DrawRowTextRightAligned(UCanvas* Canvas, const FString& Text, const float RightX, const float Y, const FLinearColor& Color)
{
	DrawRowText(Canvas, Text, RightX - MeasureText(Canvas, Text), Y, Color);
}

float FJoystickDebugCanvas::MeasureText(UCanvas* Canvas, const FString& Text)
{
	float Width = 0.0f;
	float Height = 0.0f;
	Canvas->TextSize(GEngine->GetSmallFont(), Text, Width, Height);
	return Width;
}

float FJoystickDebugCanvas::MeasureHeader(UCanvas* Canvas, const FString& Title, const FString& Stats)
{
	return MeasureText(Canvas, Title) + MeasureText(Canvas, Stats) + PanelPadding * 2.0f;
}

FString FJoystickDebugCanvas::GetDeviceName(const FJoystickInstanceId& InstanceId)
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	FJoystickInformation JoystickInfo;
	if (IsValid(JoystickSubsystem) && JoystickSubsystem->GetJoystickInfo(InstanceId, JoystickInfo))
	{
		return JoystickInfo.GetDeviceDisplayName();
	}

	return FString::Printf(TEXT("Device %d"), InstanceId.Value);
}

FLinearColor FJoystickDebugCanvas::GetThemeColor(const FSlateColor& Color, const float Opacity)
{
	return Color.GetSpecifiedColor().CopyWithNewOpacity(Opacity);
}

FLinearColor FJoystickDebugCanvas::GetTextColor(const bool IsActive)
{
	return GetThemeColor(FStyleColors::Foreground, IsActive ? 1.0f : MutedTextOpacity);
}

FLinearColor FJoystickDebugCanvas::GetLevelColor(const float Level, const bool IsClipping)
{
	if (IsClipping)
	{
		return ClippingColor;
	}

	return Level >= 0.0f ? PositiveColor : NegativeColor;
}

float FJoystickDebugCanvas::GetRowHeight()
{
	return GEngine->GetSmallFont()->GetMaxCharHeight() + RowPadding;
}

float FJoystickDebugCanvas::GetHeaderHeight()
{
	return GetRowHeight() + HeaderPadding * 2.0f;
}

bool FJoystickDebugCanvas::IsBackgroundEnabled()
{
	return CVarBackground.GetValueOnGameThread();
}

float FJoystickDebugCanvas::DrawBar(UCanvas* Canvas, const FString& Title, const FString& Stats, const FSlateColor& BackgroundColor, const float Width, const float Y)
{
	DrawPanelTile(Canvas, FVector2D(OriginX, Y), FVector2D(Width, GetHeaderHeight()), GetThemeColor(BackgroundColor));
	DrawRowText(Canvas, Title, ContentX, Y + HeaderPadding, GetThemeColor(FStyleColors::ForegroundHeader, 1.0f));
	DrawRowTextRightAligned(Canvas, Stats, OriginX + Width - PanelPadding, Y + HeaderPadding, GetTextColor(false));
	return Y + GetHeaderHeight();
}

TAutoConsoleVariable<bool> FJoystickDebugCanvas::CVarBackground(TEXT("Joystick.DebugBackground"), true, TEXT("Draws the background panels behind the joystick debug overlays. Set to 0 to see more of the game."), ECVF_Default);
