// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/ForceFeedbackDebugOverlay.h"

#include "CanvasItem.h"
#include "Debug/JoystickDebugCanvas.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "ForceFeedback/Debug/ForceFeedbackDebug.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Styling/StyleColors.h"
#include "UObject/UObjectIterator.h"

FForceFeedbackDebugOverlay::FForceFeedbackDebugOverlay()
	: FJoystickDebugOverlay(TEXT("Joystick Force Feedback"), TEXT("Joystick.FFB.Debug"), TEXT("No force feedback effects"))
{
}

bool FForceFeedbackDebugOverlay::IsEnabled() const
{
	return FForceFeedbackDebug::IsEnabled();
}

int32 FForceFeedbackDebugOverlay::GatherDevices(const UWorld* World)
{
	Devices.Reset();

	for (const UJoystickForceFeedbackComponent* Component : TObjectRange<UJoystickForceFeedbackComponent>())
	{
		if (IsValid(Component) == false || Component->GetWorld() != World)
		{
			continue;
		}

		TArray<FForceFeedbackEffectDebugInfo> Infos;
		Component->DescribeEffects(Infos);

		for (FForceFeedbackEffectDebugInfo& Info : Infos)
		{
			FForceFeedbackDebugDeviceBlock& Device = Devices.FindOrAdd(Info.InstanceId);
			Device.WritesPerSecond += Info.WritesPerSecond;
			if (Info.IsRunning && Info.IsCondition == false)
			{
				Device.EstimatedTotal += Info.EvaluatedLevel;
			}

			Device.Effects.Add(MoveTemp(Info));
		}
	}

	RecordHistory();
	return Devices.Num();
}

float FForceFeedbackDebugOverlay::DrawDevices(UCanvas* Canvas, const float Width, float Y) const
{
	for (const TPair<FJoystickInstanceId, FForceFeedbackDebugDeviceBlock>& Device : Devices)
	{
		Y = DrawDevice(Canvas, Device.Key, Device.Value, Width, Y) + FJoystickDebugCanvas::BlockSpacing;
	}

	return Y;
}

void FForceFeedbackDebugOverlay::RecordHistory()
{
	const uint64 Frame = GFrameCounter;
	for (const TPair<FJoystickInstanceId, FForceFeedbackDebugDeviceBlock>& Device : Devices)
	{
		for (const FForceFeedbackEffectDebugInfo& Info : Device.Value.Effects)
		{
			FForceFeedbackDebugEffectHistory& History = Histories.FindOrAdd(Info.Effect);
			if (History.LastFrame == Frame)
			{
				continue;
			}

			if (History.Samples.Num() != HistoryLength)
			{
				History.Samples.SetNumZeroed(HistoryLength);
			}

			History.Samples[History.Head] = FMath::Clamp(Info.Level, -1.0f, 1.0f);
			History.Head = (History.Head + 1) % HistoryLength;
			History.LastFrame = Frame;
		}
	}

	for (auto It = Histories.CreateIterator(); It; ++It)
	{
		if (It.Value().LastFrame != Frame)
		{
			It.RemoveCurrent();
		}
	}
}

void FForceFeedbackDebugOverlay::UpdateFontMetrics(UCanvas* Canvas)
{
	FontMetrics.MinLabelWidth = FMath::Max(FJoystickDebugCanvas::MeasureText(Canvas, FString::ChrN(MinLabelChars, 'S')), FJoystickDebugCanvas::MeasureText(Canvas, EstimatedTotalText));
	FontMetrics.LevelWidth = FJoystickDebugCanvas::MeasureText(Canvas, LevelSampleText);
	FontMetrics.MinKindWidth = FMath::Max(FJoystickDebugCanvas::MeasureText(Canvas, KindSampleText), FJoystickDebugCanvas::MeasureText(Canvas, ClippingText));
}

float FForceFeedbackDebugOverlay::BuildLayout(UCanvas* Canvas)
{
	float LabelWidth = FontMetrics.MinLabelWidth;
	float KindWidth = FontMetrics.MinKindWidth;
	float ValuesWidth = 0.0f;
	float HeaderWidth = 0.0f;

	for (const TPair<FJoystickInstanceId, FForceFeedbackDebugDeviceBlock>& Device : Devices)
	{
		HeaderWidth = FMath::Max(HeaderWidth, FJoystickDebugCanvas::MeasureHeader(Canvas, FJoystickDebugCanvas::GetDeviceName(Device.Key), FormatDeviceStats(Device.Key, Device.Value)));

		for (const FForceFeedbackEffectDebugInfo& Info : Device.Value.Effects)
		{
			LabelWidth = FMath::Max(LabelWidth, FJoystickDebugCanvas::MeasureText(Canvas, Info.Label));
			ValuesWidth = FMath::Max(ValuesWidth, FJoystickDebugCanvas::MeasureText(Canvas, FormatValues(Info)));

			if (Info.IsCondition == false)
			{
				KindWidth = FMath::Max(KindWidth, FJoystickDebugCanvas::MeasureText(Canvas, FormatKind(Info)));
			}
		}
	}

	constexpr float Gap = FJoystickDebugCanvas::ColumnGap;
	Layout.Dot = LabelWidth + Gap;
	Layout.Bar = Layout.Dot + DotSize + Gap;
	Layout.Level = Layout.Bar + LevelBarWidth + Gap;
	Layout.Kind = Layout.Level + FontMetrics.LevelWidth + Gap;
	Layout.Sparkline = Layout.Kind + KindWidth + Gap;
	Layout.Values = Layout.Sparkline + SparklineWidth + Gap;

	return GetPanelWidth(FMath::Max(HeaderWidth, Layout.Values + ValuesWidth));
}

float FForceFeedbackDebugOverlay::DrawDevice(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FForceFeedbackDebugDeviceBlock& Device, const float Width, float Y) const
{
	const float RowHeight = FJoystickDebugCanvas::GetRowHeight();

	Y = FJoystickDebugCanvas::DrawHeader(Canvas, FJoystickDebugCanvas::GetDeviceName(InstanceId), FormatDeviceStats(InstanceId, Device), Width, Y);
	Y = FJoystickDebugCanvas::DrawSectionBody(Canvas, Device.Effects.Num() + 1, Width, Y, TotalRowPadding);

	for (const FForceFeedbackEffectDebugInfo& Info : Device.Effects)
	{
		DrawEffectRow(Canvas, Info, Y);
		Y += RowHeight;
	}

	DrawTotalRow(Canvas, Device, Width, Y);
	return Y + TotalRowPadding + RowHeight + FJoystickDebugCanvas::PanelPadding;
}

void FForceFeedbackDebugOverlay::DrawEffectRow(UCanvas* Canvas, const FForceFeedbackEffectDebugInfo& Info, const float Y) const
{
	constexpr float ContentX = FJoystickDebugCanvas::ContentX;
	const FLinearColor RowColor = FJoystickDebugCanvas::GetTextColor(Info.IsRunning);

	FJoystickDebugCanvas::DrawRowText(Canvas, Info.Label, ContentX, Y, RowColor);
	DrawStateDot(Canvas, Info.IsRunning, Y);

	if (Info.IsCondition)
	{
		FJoystickDebugCanvas::DrawRowText(Canvas, Info.Kind.ToString(), ContentX + Layout.Bar, Y, RowColor);
	}
	else
	{
		DrawLevelBar(Canvas, ContentX + Layout.Bar, Y, Info.Level, false);
		FJoystickDebugCanvas::DrawRowText(Canvas, FormatLevel(Info.Level), ContentX + Layout.Level, Y, RowColor);
		FJoystickDebugCanvas::DrawRowText(Canvas, FormatKind(Info), ContentX + Layout.Kind, Y, RowColor);

		if (const FForceFeedbackDebugEffectHistory* History = Histories.Find(Info.Effect))
		{
			DrawSparkline(Canvas, *History, Y);
		}
	}

	FJoystickDebugCanvas::DrawRowText(Canvas, FormatValues(Info), ContentX + Layout.Values, Y, RowColor);
}

void FForceFeedbackDebugOverlay::DrawTotalRow(UCanvas* Canvas, const FForceFeedbackDebugDeviceBlock& Device, const float Width, const float DividerY) const
{
	constexpr float ContentX = FJoystickDebugCanvas::ContentX;
	const bool IsClipping = FMath::Abs(Device.EstimatedTotal) > 1.0f;
	const FLinearColor RowColor = IsClipping ? FJoystickDebugCanvas::ClippingColor : FJoystickDebugCanvas::GetTextColor(true);
	const float Y = DividerY + TotalRowPadding;

	FJoystickDebugCanvas::DrawPanelTile(Canvas, FVector2D(ContentX, DividerY), FVector2D(Width - FJoystickDebugCanvas::PanelPadding * 2.0f, 1.0f), FJoystickDebugCanvas::GetThemeColor(FStyleColors::Header, 1.0f));
	FJoystickDebugCanvas::DrawRowText(Canvas, EstimatedTotalText, ContentX, Y, RowColor);
	DrawLevelBar(Canvas, ContentX + Layout.Bar, Y, Device.EstimatedTotal, IsClipping);
	FJoystickDebugCanvas::DrawRowText(Canvas, FormatLevel(Device.EstimatedTotal), ContentX + Layout.Level, Y, RowColor);

	if (IsClipping)
	{
		FJoystickDebugCanvas::DrawRowText(Canvas, ClippingText, ContentX + Layout.Kind, Y, FJoystickDebugCanvas::ClippingColor);
	}
}

void FForceFeedbackDebugOverlay::DrawStateDot(UCanvas* Canvas, const bool IsRunning, const float Y) const
{
	const FVector2D Position(FJoystickDebugCanvas::ContentX + Layout.Dot, Y + (FJoystickDebugCanvas::GetRowHeight() - DotSize) * 0.5f);
	const FLinearColor Color = IsRunning ? FJoystickDebugCanvas::RunningColor : FJoystickDebugCanvas::GetThemeColor(FStyleColors::Hover2, 1.0f);
	FJoystickDebugCanvas::DrawTile(Canvas, Position, FVector2D(DotSize), Color);
}

void FForceFeedbackDebugOverlay::DrawSparkline(UCanvas* Canvas, const FForceFeedbackDebugEffectHistory& History, const float Y) const
{
	constexpr float BarInset = FJoystickDebugCanvas::BarInset;
	const float RowHeight = FJoystickDebugCanvas::GetRowHeight();
	const float HalfHeight = (RowHeight - BarInset * 2.0f) * 0.5f;
	const FVector2D Origin(FJoystickDebugCanvas::ContentX + Layout.Sparkline, Y + RowHeight * 0.5f);
	const FLinearColor BackgroundColor = GetRecessedColor();

	// Canvas lines are always opaque, so the glow is pre-blended over the background rather than drawn translucent.
	const FLinearColor GlowColor = FMath::Lerp(BackgroundColor, FJoystickDebugCanvas::PositiveColor, GlowStrength);

	FJoystickDebugCanvas::DrawTile(Canvas, FVector2D(Origin.X, Y + BarInset), FVector2D(SparklineWidth, HalfHeight * 2.0f), BackgroundColor);
	FJoystickDebugCanvas::DrawTile(Canvas, FVector2D(Origin.X, Origin.Y), FVector2D(SparklineWidth, 1.0f), FJoystickDebugCanvas::ZeroLineColor);

	const FVector2D PlotOrigin(Origin.X + SparklinePlotInset, Origin.Y);
	const float PlotHalfHeight = HalfHeight - SparklinePlotInset;
	DrawSparklinePass(Canvas, History, PlotOrigin, PlotHalfHeight, GlowColor, SparklineGlowThickness);
	DrawSparklinePass(Canvas, History, PlotOrigin, PlotHalfHeight, FJoystickDebugCanvas::PositiveColor, 0.0f);
}

void FForceFeedbackDebugOverlay::DrawSparklinePass(UCanvas* Canvas, const FForceFeedbackDebugEffectHistory& History, const FVector2D& Origin, const float HalfHeight, const FLinearColor& Color, const float Thickness)
{
	constexpr float Step = (SparklineWidth - SparklinePlotInset * 2.0f) / (HistoryLength - 1);

	for (int Index = 1; Index < HistoryLength; Index++)
	{
		const float Previous = History.Samples[(History.Head + Index - 1) % HistoryLength];
		const float Current = History.Samples[(History.Head + Index) % HistoryLength];

		FCanvasLineItem Line(FVector2D(Origin.X + (Index - 1) * Step, Origin.Y - Previous * HalfHeight), FVector2D(Origin.X + Index * Step, Origin.Y - Current * HalfHeight));
		Line.SetColor(Color);
		Line.LineThickness = Thickness;
		Canvas->DrawItem(Line);
	}
}

FString FForceFeedbackDebugOverlay::FormatDeviceStats(const FJoystickInstanceId& InstanceId, const FForceFeedbackDebugDeviceBlock& Device)
{
	FString EffectSlots = FString::FromInt(Device.Effects.Num());
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (IsValid(HapticDeviceManager))
	{
		EffectSlots += FString::Printf(TEXT("/%d"), HapticDeviceManager->GetMaxNumEffects(InstanceId));
	}

	return FString::Printf(TEXT("Effects %s   Writes %.0f/s"), *EffectSlots, Device.WritesPerSecond);
}

FString FForceFeedbackDebugOverlay::FormatKind(const FForceFeedbackEffectDebugInfo& Info)
{
	if (Info.FrequencyHz > 0.0f)
	{
		return FString::Printf(TEXT("%.0f Hz"), Info.FrequencyHz);
	}

	return Info.Kind.ToString();
}

FString FForceFeedbackDebugOverlay::FormatValues(const FForceFeedbackEffectDebugInfo& Info)
{
	TArray<FString> Parts;
	for (const TPair<FName, float>& Value : Info.Values)
	{
		Parts.Add(FString::Printf(TEXT("%s %.2f"), *Value.Key.ToString(), Value.Value));
	}

	for (const TPair<FName, FName>& Value : Info.TextValues)
	{
		Parts.Add(FString::Printf(TEXT("%s %s"), *Value.Key.ToString(), *Value.Value.ToString()));
	}

	return FString::Join(Parts, TEXT("   "));
}
