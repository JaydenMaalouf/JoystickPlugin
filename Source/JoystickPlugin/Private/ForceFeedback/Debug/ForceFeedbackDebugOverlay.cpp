// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Debug/ForceFeedbackDebugOverlay.h"

#if !UE_BUILD_SHIPPING

#include "CanvasItem.h"
#include "Debug/DebugDrawService.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Engine/World.h"
#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "GameFramework/PlayerController.h"
#include "JoystickSubsystem.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Misc/CoreDelegates.h"
#include "UObject/UObjectIterator.h"

void FForceFeedbackDebugOverlay::Register()
{
	if (IsValid(GEngine) && GEngine->IsInitialized())
	{
		RegisterDebugDraw();
		return;
	}

	PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FForceFeedbackDebugOverlay::RegisterDebugDraw);
}

void FForceFeedbackDebugOverlay::Unregister()
{
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
	PostEngineInitHandle.Reset();

	if (DebugDrawHandle.IsValid())
	{
		UDebugDrawService::Unregister(DebugDrawHandle);
		DebugDrawHandle.Reset();
	}

	Histories.Empty();
}

bool FForceFeedbackDebugOverlay::IsEnabled()
{
	return CVarEnabled.GetValueOnAnyThread();
}

void FForceFeedbackDebugOverlay::RegisterDebugDraw()
{
	DebugDrawHandle = UDebugDrawService::Register(TEXT("Game"), FDebugDrawDelegate::CreateRaw(this, &FForceFeedbackDebugOverlay::Draw));
}

void FForceFeedbackDebugOverlay::Draw(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (!IsEnabled() || !IsValid(Canvas) || !IsValid(PlayerController))
	{
		return;
	}

	const UWorld* World = PlayerController->GetWorld();
	if (!IsValid(World) || World->GetFirstPlayerController() != PlayerController)
	{
		return;
	}

	TMap<FJoystickInstanceId, FDeviceBlock> Devices;
	GatherDevices(World, Devices);
	RecordHistory(Devices);

	float Y = OriginY;
	DrawRowText(Canvas, TEXT("Joystick Force Feedback Effects"), OriginX, Y, TitleColor);
	Y += GetRowHeight();

	if (Devices.Num() == 0)
	{
		DrawRowText(Canvas, TEXT("No force feedback effects"), OriginX, Y, MutedColor);
		return;
	}

	for (const TPair<FJoystickInstanceId, FDeviceBlock>& Device : Devices)
	{
		Y = DrawDevice(Canvas, Device.Key, Device.Value, Y) + BlockSpacing;
	}
}

void FForceFeedbackDebugOverlay::RecordHistory(const TMap<FJoystickInstanceId, FDeviceBlock>& Devices)
{
	const uint64 Frame = GFrameCounter;
	for (const TPair<FJoystickInstanceId, FDeviceBlock>& Device : Devices)
	{
		for (const FForceFeedbackEffectDebugInfo& Info : Device.Value.Effects)
		{
			FEffectHistory& History = Histories.FindOrAdd(Info.Effect);
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

float FForceFeedbackDebugOverlay::DrawDevice(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FDeviceBlock& Device, float Y) const
{
	const float RowHeight = GetRowHeight();

	DrawDeviceHeader(Canvas, InstanceId, Device, Y);
	Y += RowHeight;

	for (const FForceFeedbackEffectDebugInfo& Info : Device.Effects)
	{
		DrawEffectRow(Canvas, Info, Y);
		Y += RowHeight;
	}

	DrawTotalRow(Canvas, Device, Y);
	return Y + RowHeight;
}

void FForceFeedbackDebugOverlay::DrawEffectRow(UCanvas* Canvas, const FForceFeedbackEffectDebugInfo& Info, const float Y) const
{
	const FLinearColor& RowColor = Info.IsRunning ? TextColor : MutedColor;

	DrawRowText(Canvas, Info.Label, OriginX, Y, RowColor);
	DrawStateDot(Canvas, Info.IsRunning, Y);

	if (Info.IsCondition)
	{
		DrawRowText(Canvas, Info.Kind.ToString(), OriginX + BarColumn, Y, RowColor);
	}
	else
	{
		DrawLevelBar(Canvas, Info.Level, BarFillColor, Y);
		DrawRowText(Canvas, FString::Printf(TEXT("%+.2f"), Info.Level), OriginX + LevelColumn, Y, RowColor);
		DrawRowText(Canvas, FormatKind(Info), OriginX + KindColumn, Y, RowColor);

		if (const FEffectHistory* History = Histories.Find(Info.Effect))
		{
			DrawSparkline(Canvas, *History, Y);
		}
	}

	DrawRowText(Canvas, FormatValues(Info), OriginX + ValuesColumn, Y, RowColor);
}

void FForceFeedbackDebugOverlay::GatherDevices(const UWorld* World, TMap<FJoystickInstanceId, FDeviceBlock>& OutDevices)
{
	for (const UJoystickForceFeedbackComponent* Component : TObjectRange<UJoystickForceFeedbackComponent>())
	{
		if (!IsValid(Component) || Component->GetWorld() != World)
		{
			continue;
		}

		TArray<FForceFeedbackEffectDebugInfo> Infos;
		Component->DescribeEffects(Infos);

		for (FForceFeedbackEffectDebugInfo& Info : Infos)
		{
			FDeviceBlock& Device = OutDevices.FindOrAdd(Info.InstanceId);
			Device.WritesPerSecond += Info.WritesPerSecond;
			if (Info.IsRunning && !Info.IsCondition)
			{
				Device.EstimatedTotal += Info.EvaluatedLevel;
			}

			Device.Effects.Add(MoveTemp(Info));
		}
	}
}

void FForceFeedbackDebugOverlay::DrawDeviceHeader(UCanvas* Canvas, const FJoystickInstanceId& InstanceId, const FDeviceBlock& Device, const float Y)
{
	FString DeviceName = FString::Printf(TEXT("Device %d"), InstanceId.Value);
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	FJoystickInformation JoystickInfo;
	if (IsValid(JoystickSubsystem) && JoystickSubsystem->GetJoystickInfo(InstanceId, JoystickInfo))
	{
		DeviceName = JoystickInfo.GetDeviceDisplayName();
	}

	FString EffectSlots = FString::FromInt(Device.Effects.Num());
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (IsValid(HapticDeviceManager))
	{
		EffectSlots += FString::Printf(TEXT("/%d"), HapticDeviceManager->GetMaxNumEffects(InstanceId));
	}

	const FString Header = FString::Printf(TEXT("%s   effects %s   writes %.0f/s"), *DeviceName, *EffectSlots, Device.WritesPerSecond);
	DrawRowText(Canvas, Header, OriginX, Y, HeaderColor);
}

void FForceFeedbackDebugOverlay::DrawTotalRow(UCanvas* Canvas, const FDeviceBlock& Device, const float Y)
{
	const bool IsClipping = FMath::Abs(Device.EstimatedTotal) > 1.0f;
	const FLinearColor& RowColor = IsClipping ? ClippingColor : TextColor;

	DrawRowText(Canvas, TEXT("Estimated total"), OriginX, Y, RowColor);
	DrawLevelBar(Canvas, Device.EstimatedTotal, IsClipping ? ClippingColor : BarFillColor, Y);
	DrawRowText(Canvas, FString::Printf(TEXT("%+.2f"), Device.EstimatedTotal), OriginX + LevelColumn, Y, RowColor);
}

void FForceFeedbackDebugOverlay::DrawStateDot(UCanvas* Canvas, const bool IsRunning, const float Y)
{
	const FVector2D Position(OriginX + DotColumn, Y + (GetRowHeight() - DotSize) * 0.5f);
	DrawTile(Canvas, Position, FVector2D(DotSize), IsRunning ? RunningColor : StoppedColor);
}

void FForceFeedbackDebugOverlay::DrawLevelBar(UCanvas* Canvas, const float Level, const FLinearColor& FillColor, const float Y)
{
	const FVector2D Position(OriginX + BarColumn, Y + BarInset);
	const FVector2D Size(BarWidth, GetRowHeight() - BarInset * 2.0f);
	constexpr float HalfWidth = BarWidth * 0.5f;
	const float CentreX = Position.X + HalfWidth;
	const float Fill = FMath::Clamp(Level, -1.0f, 1.0f) * HalfWidth;

	DrawTile(Canvas, Position, Size, BarBackgroundColor);
	DrawTile(Canvas, FVector2D(CentreX + FMath::Min(Fill, 0.0f), Position.Y), FVector2D(FMath::Abs(Fill), Size.Y), FillColor);
	DrawTile(Canvas, FVector2D(CentreX, Position.Y), FVector2D(1.0f, Size.Y), BarCentreColor);
}

void FForceFeedbackDebugOverlay::DrawSparkline(UCanvas* Canvas, const FEffectHistory& History, const float Y)
{
	const float RowHeight = GetRowHeight();
	const float HalfHeight = (RowHeight - BarInset * 2.0f) * 0.5f;
	const float MidY = Y + RowHeight * 0.5f;
	constexpr float X = OriginX + SparklineColumn;
	constexpr float Step = SparklineWidth / (HistoryLength - 1);

	DrawTile(Canvas, FVector2D(X, Y + BarInset), FVector2D(SparklineWidth, HalfHeight * 2.0f), BarBackgroundColor);

	for (int32 Index = 1; Index < HistoryLength; Index++)
	{
		const float Previous = History.Samples[(History.Head + Index - 1) % HistoryLength];
		const float Current = History.Samples[(History.Head + Index) % HistoryLength];

		FCanvasLineItem Line(FVector2D(X + (Index - 1) * Step, MidY - Previous * HalfHeight), FVector2D(X + Index * Step, MidY - Current * HalfHeight));
		Line.SetColor(SparklineColor);
		Canvas->DrawItem(Line);
	}
}

void FForceFeedbackDebugOverlay::DrawTile(UCanvas* Canvas, const FVector2D& Position, const FVector2D& Size, const FLinearColor& Color)
{
	FCanvasTileItem Tile(Position, Size, Color);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}

void FForceFeedbackDebugOverlay::DrawRowText(UCanvas* Canvas, const FString& Text, const float X, const float Y, const FLinearColor& Color)
{
	FFontRenderInfo RenderInfo;
	RenderInfo.bEnableShadow = true;

	Canvas->SetDrawColor(Color.ToFColor(true));
	Canvas->DrawText(GEngine->GetSmallFont(), Text, X, Y, 1.0f, 1.0f, RenderInfo);
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

float FForceFeedbackDebugOverlay::GetRowHeight()
{
	return GEngine->GetSmallFont()->GetMaxCharHeight() + RowPadding;
}

TAutoConsoleVariable<bool> FForceFeedbackDebugOverlay::CVarEnabled(TEXT("Joystick.FFB.Debug"), false, TEXT("Shows what every force feedback effect is sending to each device."), ECVF_Default);

#endif
