// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/IConsoleManager.h"

class UCanvas;
struct FJoystickInstanceId;
struct FSlateColor;

// The panels, rows and text the joystick debug overlays are drawn from.
class FJoystickDebugCanvas
{
public:
	static float DrawTitleBar(UCanvas* Canvas, const FString& Title, const FString& Hint, float Width, float Y);
	static float DrawHeader(UCanvas* Canvas, const FString& Title, const FString& Stats, float Width, float Y);
	static float DrawSectionBody(UCanvas* Canvas, int32 NumRows, float Width, float Y, float ExtraHeight = 0.0f);
	static float DrawEmptyBody(UCanvas* Canvas, const FString& Text, float Width, float Y);
	static void DrawPanelTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color);
	static void DrawTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color);
	static void DrawRowText(UCanvas* Canvas, const FString& Text, float X, float Y, const FLinearColor& Color);
	static void DrawRowTextRightAligned(UCanvas* Canvas, const FString& Text, float RightX, float Y, const FLinearColor& Color);
	static float MeasureText(UCanvas* Canvas, const FString& Text);
	static float MeasureHeader(UCanvas* Canvas, const FString& Title, const FString& Stats);
	static FString GetDeviceName(const FJoystickInstanceId& InstanceId);
	static FLinearColor GetThemeColor(const FSlateColor& Color, float Opacity = PanelOpacity);
	static FLinearColor GetTextColor(bool IsActive);
	static FLinearColor GetLevelColor(float Level, bool IsClipping);
	static float GetRowHeight();
	static float GetHeaderHeight();
	static bool IsBackgroundEnabled();

	static constexpr float OriginX = 40.0f;
	static constexpr float OriginY = 40.0f;
	static constexpr float PanelPadding = 10.0f;
	static constexpr float ContentX = OriginX + PanelPadding;
	static constexpr float HeaderPadding = 5.0f;
	static constexpr float RowPadding = 4.0f;
	static constexpr float BlockSpacing = 2.0f;
	static constexpr float OverlaySpacing = 12.0f;
	static constexpr float PanelOpacity = 0.94f;
	static constexpr float BarInset = 4.0f;
	static constexpr float ColumnGap = 12.0f;
	static constexpr float MutedTextOpacity = 0.45f;

	// Mirrors the Haptic Effect Editor's palette in FJoystickPluginEditorStyle, which this module can't reach.
	static constexpr FLinearColor PositiveColor = FLinearColor(0.2f, 0.9f, 0.3f);
	static constexpr FLinearColor NegativeColor = FLinearColor(0.25f, 0.7f, 1.0f);
	static constexpr FLinearColor RunningColor = FLinearColor(0.2f, 1.0f, 0.2f);
	static constexpr FLinearColor ClippingColor = FLinearColor(0.9f, 0.3f, 0.3f);
	static constexpr FLinearColor ZeroLineColor = FLinearColor(0.4f, 0.4f, 0.4f);

private:
	static float DrawBar(UCanvas* Canvas, const FString& Title, const FString& Stats, const FSlateColor& BackgroundColor, float Width, float Y);

	static TAutoConsoleVariable<bool> CVarBackground;
};
