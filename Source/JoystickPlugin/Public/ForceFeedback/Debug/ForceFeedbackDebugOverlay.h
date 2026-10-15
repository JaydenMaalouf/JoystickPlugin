// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if !UE_BUILD_SHIPPING

#include "HAL/IConsoleManager.h"
#include "ForceFeedback/Debug/ForceFeedbackEffectDebugInfo.h"

class APlayerController;
class UCanvas;
class UWorld;

// Draws what every force feedback effect is sending to each device, toggled with Joystick.FFB.Debug.
class JOYSTICKPLUGIN_API FForceFeedbackDebugOverlay
{
public:
	void Register();
	void Unregister();

	static bool IsEnabled();

private:
	struct FDeviceBlock
	{
		TArray<FForceFeedbackEffectDebugInfo> Effects;
		float WritesPerSecond = 0.0f;
		float EstimatedTotal = 0.0f;
	};

	struct FEffectHistory
	{
		TArray<float> Samples;
		int32 Head = 0;
		uint64 LastFrame = 0;
	};

	void RegisterDebugDraw();
	void Draw(UCanvas* Canvas, APlayerController* PlayerController);
	void RecordHistory(const TMap<FJoystickInstanceId, FDeviceBlock>& Devices);
	float DrawDevice(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FDeviceBlock& Device, float Y) const;
	void DrawEffectRow(UCanvas* Canvas, const FForceFeedbackEffectDebugInfo& Info, float Y) const;

	static void GatherDevices(const UWorld* World, TMap<FJoystickInstanceId, FDeviceBlock>& OutDevices);
	static void DrawDeviceHeader(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FDeviceBlock& Device, float Y);
	static void DrawTotalRow(UCanvas* Canvas, const FDeviceBlock& Device, float Y);
	static void DrawStateDot(UCanvas* Canvas, bool IsRunning, float Y);
	static void DrawLevelBar(UCanvas* Canvas, float Level, const FLinearColor& FillColor, float Y);
	static void DrawSparkline(UCanvas* Canvas, const FEffectHistory& History, float Y);
	static void DrawTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color);
	static void DrawRowText(UCanvas* Canvas, const FString& Text, float X, float Y, const FLinearColor& Color);
	static FString FormatKind(const FForceFeedbackEffectDebugInfo& Info);
	static FString FormatValues(const FForceFeedbackEffectDebugInfo& Info);
	static float GetRowHeight();

	static TAutoConsoleVariable<bool> CVarEnabled;

	static constexpr float OriginX = 40.0f;
	static constexpr float OriginY = 120.0f;
	static constexpr float RowPadding = 2.0f;
	static constexpr float BlockSpacing = 12.0f;
	static constexpr float BarInset = 3.0f;
	static constexpr float DotSize = 8.0f;
	static constexpr float DotColumn = 280.0f;
	static constexpr float BarColumn = 296.0f;
	static constexpr float BarWidth = 160.0f;
	static constexpr float LevelColumn = 468.0f;
	static constexpr float KindColumn = 524.0f;
	static constexpr float SparklineColumn = 604.0f;
	static constexpr float SparklineWidth = 128.0f;
	static constexpr float ValuesColumn = 744.0f;
	static constexpr int32 HistoryLength = 128;

	static constexpr FLinearColor TitleColor = FLinearColor(1.0f, 0.85f, 0.3f);
	static constexpr FLinearColor HeaderColor = FLinearColor(0.55f, 0.8f, 1.0f);
	static constexpr FLinearColor TextColor = FLinearColor(0.9f, 0.9f, 0.9f);
	static constexpr FLinearColor MutedColor = FLinearColor(0.5f, 0.5f, 0.5f);
	static constexpr FLinearColor RunningColor = FLinearColor(0.3f, 0.9f, 0.4f);
	static constexpr FLinearColor StoppedColor = FLinearColor(0.35f, 0.35f, 0.35f);
	static constexpr FLinearColor BarBackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);
	static constexpr FLinearColor BarFillColor = FLinearColor(0.3f, 0.65f, 1.0f);
	static constexpr FLinearColor BarCentreColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.6f);
	static constexpr FLinearColor SparklineColor = FLinearColor(0.3f, 0.65f, 1.0f, 0.8f);
	static constexpr FLinearColor ClippingColor = FLinearColor(1.0f, 0.25f, 0.2f);

	FDelegateHandle PostEngineInitHandle;
	FDelegateHandle DebugDrawHandle;
	TMap<FObjectKey, FEffectHistory> Histories;
};

#endif
