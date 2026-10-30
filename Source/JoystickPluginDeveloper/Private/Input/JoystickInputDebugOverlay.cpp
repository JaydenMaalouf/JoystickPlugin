// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Input/JoystickInputDebugOverlay.h"

#include "Data/JoystickInformation.h"
#include "Data/Input/HatDirection.h"
#include "Debug/JoystickDebugCanvas.h"
#include "Engine/Engine.h"
#include "JoystickInputSettings.h"
#include "JoystickSubsystem.h"

FJoystickInputDebugOverlay::FJoystickInputDebugOverlay()
	: FJoystickDebugOverlay(TEXT("Joystick Input"), TEXT("Joystick.Input.Debug"), TEXT("No joysticks connected"))
{
}

bool FJoystickInputDebugOverlay::IsEnabled() const
{
	return CVarEnabled.GetValueOnGameThread();
}

int32 FJoystickInputDebugOverlay::GatherDevices(const UWorld* World)
{
	Devices.Reset();

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return 0;
	}

	TArray<FJoystickInstanceId> InstanceIds;
	JoystickSubsystem->GetInstanceIds(InstanceIds);

	for (const FJoystickInstanceId& InstanceId : InstanceIds)
	{
		FJoystickInputDebugDeviceBlock Device;
		FJoystickInformation Info;
		if (JoystickSubsystem->GetJoystickState(InstanceId, Device.State) == false || JoystickSubsystem->GetJoystickInfo(InstanceId, Info) == false)
		{
			continue;
		}

		Device.Name = Info.GetDeviceDisplayName();
		AddTextRows(Info, Device);
		Devices.Add(MoveTemp(Device));
	}

	return Devices.Num();
}

float FJoystickInputDebugOverlay::DrawDevices(UCanvas* Canvas, const float Width, float Y) const
{
	for (const FJoystickInputDebugDeviceBlock& Device : Devices)
	{
		Y = DrawDevice(Canvas, Device, Width, Y) + FJoystickDebugCanvas::BlockSpacing;
	}

	return Y;
}

void FJoystickInputDebugOverlay::UpdateFontMetrics(UCanvas* Canvas)
{
	FontMetrics.MinLabelWidth = FMath::Max(FJoystickDebugCanvas::MeasureText(Canvas, AxisLabelSampleText), FJoystickDebugCanvas::MeasureText(Canvas, ButtonsLabelText));
	FontMetrics.ValueWidth = FJoystickDebugCanvas::MeasureText(Canvas, LevelSampleText);
	FontMetrics.RawWidth = FJoystickDebugCanvas::MeasureText(Canvas, RawSampleText);
	FontMetrics.ButtonCellWidth = FJoystickDebugCanvas::MeasureText(Canvas, ButtonCellSampleText) + ButtonCellPadding * 2.0f;
}

float FJoystickInputDebugOverlay::BuildLayout(UCanvas* Canvas)
{
	float LabelWidth = FontMetrics.MinLabelWidth;
	float TextWidth = 0.0f;
	float HeaderWidth = 0.0f;
	int32 MaxButtons = 0;

	for (const FJoystickInputDebugDeviceBlock& Device : Devices)
	{
		HeaderWidth = FMath::Max(HeaderWidth, FJoystickDebugCanvas::MeasureHeader(Canvas, Device.Name, FormatDeviceStats(Device)));
		MaxButtons = FMath::Max(MaxButtons, Device.State.Buttons.Num());

		for (const FJoystickInputDebugTextRow& Row : Device.TextRows)
		{
			LabelWidth = FMath::Max(LabelWidth, FJoystickDebugCanvas::MeasureText(Canvas, Row.Label));
			TextWidth = FMath::Max(TextWidth, FJoystickDebugCanvas::MeasureText(Canvas, Row.Text));
		}
	}

	constexpr float Gap = FJoystickDebugCanvas::ColumnGap;
	Layout.Bar = LabelWidth + Gap;
	Layout.Value = Layout.Bar + LevelBarWidth + Gap;
	Layout.Raw = Layout.Value + FontMetrics.ValueWidth + Gap;

	const float AxisWidth = Layout.Raw + FontMetrics.RawWidth;
	const float ButtonWidth = Layout.Bar + GetButtonGridWidth(MaxButtons);
	return GetPanelWidth(FMath::Max(FMath::Max(HeaderWidth, AxisWidth), FMath::Max(ButtonWidth, Layout.Bar + TextWidth)));
}

float FJoystickInputDebugOverlay::DrawDevice(UCanvas* Canvas, const FJoystickInputDebugDeviceBlock& Device, const float Width, float Y) const
{
	Y = FJoystickDebugCanvas::DrawHeader(Canvas, Device.Name, FormatDeviceStats(Device), Width, Y);
	Y = FJoystickDebugCanvas::DrawSectionBody(Canvas, GetRowCount(Device), Width, Y);
	Y = DrawAxisRows(Canvas, Device.State, Y);
	Y = DrawButtonRows(Canvas, Device.State, Y);
	Y = DrawTextRows(Canvas, Device.TextRows, Y);
	return Y + FJoystickDebugCanvas::PanelPadding;
}

float FJoystickInputDebugOverlay::DrawAxisRows(UCanvas* Canvas, const FJoystickDeviceState& State, float Y) const
{
	constexpr float ContentX = FJoystickDebugCanvas::ContentX;
	const float RowHeight = FJoystickDebugCanvas::GetRowHeight();
	const FLinearColor TextColor = FJoystickDebugCanvas::GetTextColor(true);

	for (int32 Index = 0; Index < State.Axes.Num(); Index++)
	{
		const FAxisData& Axis = State.Axes[Index];
		const float Value = Axis.GetValue();

		FJoystickDebugCanvas::DrawRowText(Canvas, FString::Printf(TEXT("Axis %d"), GetDisplayIndex(Index)), ContentX, Y, TextColor);
		DrawLevelBar(Canvas, ContentX + Layout.Bar, Y, Value, false);
		FJoystickDebugCanvas::DrawRowText(Canvas, FormatLevel(Value), ContentX + Layout.Value, Y, TextColor);

		if (Axis.RemappingEnabled)
		{
			FJoystickDebugCanvas::DrawRowText(Canvas, TEXT("Raw ") + FormatLevel(Axis.Value), ContentX + Layout.Raw, Y, FJoystickDebugCanvas::GetTextColor(false));
		}

		Y += RowHeight;
	}

	return Y;
}

float FJoystickInputDebugOverlay::DrawButtonRows(UCanvas* Canvas, const FJoystickDeviceState& State, const float Y) const
{
	const int32 NumButtons = State.Buttons.Num();
	if (NumButtons == 0)
	{
		return Y;
	}

	const float RowHeight = FJoystickDebugCanvas::GetRowHeight();
	const float GridX = FJoystickDebugCanvas::ContentX + Layout.Bar;
	FJoystickDebugCanvas::DrawRowText(Canvas, ButtonsLabelText, FJoystickDebugCanvas::ContentX, Y, FJoystickDebugCanvas::GetTextColor(true));

	for (int32 Index = 0; Index < NumButtons; Index++)
	{
		const float X = GridX + (Index % ButtonsPerRow) * (FontMetrics.ButtonCellWidth + ButtonCellSpacing);
		DrawButtonCell(Canvas, Index, State.Buttons[Index].GetValue(), X, Y + (Index / ButtonsPerRow) * RowHeight);
	}

	return Y + GetButtonRowCount(NumButtons) * RowHeight;
}

void FJoystickInputDebugOverlay::DrawButtonCell(UCanvas* Canvas, const int32 ButtonIndex, const bool IsPressed, const float X, const float Y) const
{
	const FVector2D Size(FontMetrics.ButtonCellWidth, FJoystickDebugCanvas::GetRowHeight() - ButtonCellSpacing);
	const FLinearColor Color = IsPressed ? FJoystickDebugCanvas::PositiveColor : GetRecessedColor();

	FJoystickDebugCanvas::DrawTile(Canvas, FVector2D(X, Y), Size, Color);
	FJoystickDebugCanvas::DrawRowText(Canvas, FString::FromInt(GetDisplayIndex(ButtonIndex)), X + ButtonCellPadding, Y, FJoystickDebugCanvas::GetTextColor(IsPressed));
}

float FJoystickInputDebugOverlay::DrawTextRows(UCanvas* Canvas, const TArray<FJoystickInputDebugTextRow>& TextRows, float Y) const
{
	constexpr float ContentX = FJoystickDebugCanvas::ContentX;
	const float RowHeight = FJoystickDebugCanvas::GetRowHeight();

	for (const FJoystickInputDebugTextRow& Row : TextRows)
	{
		FJoystickDebugCanvas::DrawRowText(Canvas, Row.Label, ContentX, Y, FJoystickDebugCanvas::GetTextColor(true));
		FJoystickDebugCanvas::DrawRowText(Canvas, Row.Text, ContentX + Layout.Bar, Y, FJoystickDebugCanvas::GetTextColor(Row.IsActive));
		Y += RowHeight;
	}

	return Y;
}

float FJoystickInputDebugOverlay::GetButtonGridWidth(const int32 NumButtons) const
{
	return FMath::Min(NumButtons, ButtonsPerRow) * (FontMetrics.ButtonCellWidth + ButtonCellSpacing);
}

void FJoystickInputDebugOverlay::AddTextRows(const FJoystickInformation& Info, FJoystickInputDebugDeviceBlock& Device)
{
	const FJoystickDeviceState& State = Device.State;

	for (int32 Index = 0; Index < State.Hats.Num(); Index++)
	{
		const EHatDirection Direction = State.Hats[Index].GetValue();
		Device.TextRows.Add({FString::Printf(TEXT("Hat %d"), GetDisplayIndex(Index)), FormatHat(Direction), Direction != EHatDirection::None});
	}

	for (int32 Index = 0; Index < State.Balls.Num(); Index++)
	{
		const FVector2D& Direction = State.Balls[Index].Direction;
		Device.TextRows.Add({FString::Printf(TEXT("Ball %d"), GetDisplayIndex(Index)), FormatVector2D(Direction), Direction.IsNearlyZero() == false});
	}

	if (Info.Gyro.Enabled)
	{
		Device.TextRows.Add({TEXT("Gyro"), FormatVector(State.Motion.Gyro), true});
	}

	if (Info.Accelerometer.Enabled)
	{
		Device.TextRows.Add({TEXT("Accelerometer"), FormatVector(State.Motion.Accelerometer), true});
	}
}

int32 FJoystickInputDebugOverlay::GetButtonRowCount(const int32 NumButtons)
{
	return FMath::DivideAndRoundUp(NumButtons, ButtonsPerRow);
}

int32 FJoystickInputDebugOverlay::GetRowCount(const FJoystickInputDebugDeviceBlock& Device)
{
	return Device.State.Axes.Num() + GetButtonRowCount(Device.State.Buttons.Num()) + Device.TextRows.Num();
}

int32 FJoystickInputDebugOverlay::GetDisplayIndex(const int32 Index)
{
	return GetDefault<UJoystickInputSettings>()->ZeroBasedIndexing ? Index : Index + 1;
}

FString FJoystickInputDebugOverlay::FormatDeviceStats(const FJoystickInputDebugDeviceBlock& Device)
{
	const FJoystickDeviceState& State = Device.State;
	return FString::Printf(TEXT("Axes %d   Buttons %d   Hats %d"), State.Axes.Num(), State.Buttons.Num(), State.Hats.Num());
}

FString FJoystickInputDebugOverlay::FormatHat(const EHatDirection Direction)
{
	if (Direction == EHatDirection::None)
	{
		return TEXT("Centred");
	}

	return StaticEnum<EHatDirection>()->GetNameStringByValue(static_cast<int64>(Direction)).Replace(TEXT("_"), TEXT(" "));
}

FString FJoystickInputDebugOverlay::FormatVector(const FVector& Vector)
{
	return FString::Printf(TEXT("X %+.2f   Y %+.2f   Z %+.2f"), Vector.X, Vector.Y, Vector.Z);
}

FString FJoystickInputDebugOverlay::FormatVector2D(const FVector2D& Vector)
{
	return FString::Printf(TEXT("X %+.2f   Y %+.2f"), Vector.X, Vector.Y);
}

TAutoConsoleVariable<bool> FJoystickInputDebugOverlay::CVarEnabled(TEXT("Joystick.Input.Debug"), false, TEXT("Shows the live axes, buttons, hats and sensors of every connected joystick."), ECVF_Default);
