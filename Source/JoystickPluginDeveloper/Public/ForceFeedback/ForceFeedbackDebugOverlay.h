// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Debug/JoystickDebugOverlay.h"
#include "ForceFeedback/Data/ForceFeedbackDebugColumnLayout.h"
#include "ForceFeedback/Data/ForceFeedbackDebugDeviceBlock.h"
#include "ForceFeedback/Data/ForceFeedbackDebugEffectHistory.h"
#include "ForceFeedback/Data/ForceFeedbackDebugFontMetrics.h"

// Draws what every force feedback effect is sending to each device, toggled with Joystick.FFB.Debug.
class FForceFeedbackDebugOverlay final : public FJoystickDebugOverlay
{
public:
	FForceFeedbackDebugOverlay();

	virtual bool IsEnabled() const override;

private:
	virtual int32 GatherDevices(const UWorld* World) override;
	virtual void UpdateFontMetrics(UCanvas* Canvas) override;
	virtual float BuildLayout(UCanvas* Canvas) override;
	virtual float DrawDevices(UCanvas* Canvas, float Width, float Y) const override;

	void RecordHistory();
	float DrawDevice(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FForceFeedbackDebugDeviceBlock& Device, float Width, float Y) const;
	void DrawEffectRow(UCanvas* Canvas, const FForceFeedbackEffectDebugInfo& Info, float Y) const;
	void DrawTotalRow(UCanvas* Canvas, const FForceFeedbackDebugDeviceBlock& Device, float Width, float DividerY) const;
	void DrawStateDot(UCanvas* Canvas, bool IsRunning, float Y) const;
	void DrawSparkline(UCanvas* Canvas, const FForceFeedbackDebugEffectHistory& History, float Y) const;

	static void DrawSparklinePass(UCanvas* Canvas, const FForceFeedbackDebugEffectHistory& History, const FVector2D& Origin, float HalfHeight, const FLinearColor& Color, float Thickness);
	static FString FormatDeviceStats(const FJoystickInstanceId& InstanceId, const FForceFeedbackDebugDeviceBlock& Device);
	static FString FormatKind(const FForceFeedbackEffectDebugInfo& Info);
	static FString FormatValues(const FForceFeedbackEffectDebugInfo& Info);

	static constexpr float TotalRowPadding = 4.0f;
	static constexpr float DotSize = 6.0f;
	static constexpr float SparklineWidth = 128.0f;
	static constexpr float SparklineGlowThickness = 3.0f;
	static constexpr float SparklinePlotInset = SparklineGlowThickness * 0.5f;
	static constexpr float GlowStrength = 0.22f;
	static constexpr int32 HistoryLength = 128;
	static constexpr int32 MinLabelChars = 32;
	static constexpr const TCHAR* KindSampleText = TEXT("000 Hz");
	static constexpr const TCHAR* EstimatedTotalText = TEXT("Estimated total");
	static constexpr const TCHAR* ClippingText = TEXT("Clipping");

	TMap<FJoystickInstanceId, FForceFeedbackDebugDeviceBlock> Devices;
	TMap<FObjectKey, FForceFeedbackDebugEffectHistory> Histories;
	FForceFeedbackDebugFontMetrics FontMetrics;
	FForceFeedbackDebugColumnLayout Layout;
};
