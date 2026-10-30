// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/JoystickDeviceState.h"
#include "Debug/JoystickDebugOverlay.h"
#include "HAL/IConsoleManager.h"
#include "Input/Data/JoystickInputDebugColumnLayout.h"
#include "Input/Data/JoystickInputDebugDeviceBlock.h"
#include "Input/Data/JoystickInputDebugFontMetrics.h"
#include "Input/Data/JoystickInputDebugTextRow.h"

enum class EHatDirection : uint8;
struct FJoystickInformation;

// Draws every connected device's live axes, buttons, hats, balls and motion sensors, toggled with Joystick.Input.Debug.
class FJoystickInputDebugOverlay final : public FJoystickDebugOverlay
{
public:
	FJoystickInputDebugOverlay();

	virtual bool IsEnabled() const override;

private:
	virtual int32 GatherDevices(const UWorld* World) override;
	virtual void UpdateFontMetrics(UCanvas* Canvas) override;
	virtual float BuildLayout(UCanvas* Canvas) override;
	virtual float DrawDevices(UCanvas* Canvas, float Width, float Y) const override;

	float DrawDevice(UCanvas* Canvas, const FJoystickInputDebugDeviceBlock& Device, float Width, float Y) const;
	float DrawAxisRows(UCanvas* Canvas, const FJoystickDeviceState& State, float Y) const;
	float DrawButtonRows(UCanvas* Canvas, const FJoystickDeviceState& State, float Y) const;
	void DrawButtonCell(UCanvas* Canvas, int32 ButtonIndex, bool IsPressed, float X, float Y) const;
	float DrawTextRows(UCanvas* Canvas, const TArray<FJoystickInputDebugTextRow>& TextRows, float Y) const;
	float GetButtonGridWidth(int32 NumButtons) const;

	static void AddTextRows(const FJoystickInformation& Info, FJoystickInputDebugDeviceBlock& Device);
	static int32 GetButtonRowCount(int32 NumButtons);
	static int32 GetRowCount(const FJoystickInputDebugDeviceBlock& Device);
	static int32 GetDisplayIndex(int32 Index);
	static FString FormatDeviceStats(const FJoystickInputDebugDeviceBlock& Device);
	static FString FormatHat(EHatDirection Direction);
	static FString FormatVector(const FVector& Vector);
	static FString FormatVector2D(const FVector2D& Vector);

	static constexpr float ButtonCellPadding = 3.0f;
	static constexpr float ButtonCellSpacing = 2.0f;
	static constexpr int32 ButtonsPerRow = 16;
	static constexpr const TCHAR* AxisLabelSampleText = TEXT("Axis 00");
	static constexpr const TCHAR* ButtonsLabelText = TEXT("Buttons");
	static constexpr const TCHAR* RawSampleText = TEXT("Raw +0.00");
	static constexpr const TCHAR* ButtonCellSampleText = TEXT("000");

	static TAutoConsoleVariable<bool> CVarEnabled;

	TArray<FJoystickInputDebugDeviceBlock> Devices;
	FJoystickInputDebugFontMetrics FontMetrics;
	FJoystickInputDebugColumnLayout Layout;
};
