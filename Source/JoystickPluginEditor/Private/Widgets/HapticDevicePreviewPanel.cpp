// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticDevicePreviewPanel.h"

#include "JoystickSubsystem.h"
#include "Engine/Engine.h"
#include "Styling/AppStyle.h"
#include "Widgets/HapticDevicePreview.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

struct FHapticEffectEditorPreview
{
	// How far (in normalised -1..1 units) an axis must move during playback to be treated as the one the effect is driving.
	static constexpr float AxisDetectionThreshold = 0.01f;

	// User-entered degrees of rotation each way from centre, as no joystick API reports it.
	static constexpr float MinRotationLimitDegrees = 45.0f;
	static constexpr float MaxRotationLimitDegrees = 1440.0f;
};

void SHapticDevicePreviewPanel::Construct(const FArguments& InArgs)
{
	Joystick = InArgs._Joystick;
	IsPlaying = InArgs._IsPlaying;

	auto GetPreviewVisibility = [this]
	{
		return SHapticDevicePreview::SupportsType(PreviewJoystickType) ? EVisibility::Visible : EVisibility::Collapsed;
	};

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f, 6.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("DevicePreviewLabel", "DEVICE PREVIEW"))
					.TextStyle(FAppStyle::Get(), "SmallText")
					.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SSpacer)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Visibility_Lambda(GetPreviewVisibility)
					.Text_Lambda([this]
					{
						return FText::FromString(FString::Printf(TEXT("Axis %d   %+.1f\u00B0"), PreviewAxisIndex, GetPreviewAxisDegrees()));
					})
					.ToolTipText(LOCTEXT("DevicePreviewAxisTooltip", "The axis driving the preview. Defaults to axis 0 and switches to whichever axis moves the most while an effect plays."))
					.TextStyle(FAppStyle::Get(), "NormalText")
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(0, 6, 0, 6)
			[
				SNew(SHapticDevicePreview)
				.Visibility_Lambda(GetPreviewVisibility)
				.JoystickType_Lambda([this] { return PreviewJoystickType; })
				.AxisValue_Lambda([this] { return PreviewAxisValue; })
				.MaxRotationDegrees_Lambda([this] { return PreviewMaxRotationDegrees; })
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(0, 6, 0, 6)
			[
				SNew(STextBlock)
				.Visibility_Lambda([this]
				{
					return SHapticDevicePreview::SupportsType(PreviewJoystickType) ? EVisibility::Collapsed : EVisibility::Visible;
				})
				.Text_Lambda([this]
				{
					if (!Joystick.Get().IsValid())
					{
						return LOCTEXT("NoDevice", "No Device Selected");
					}

					return FText::Format(LOCTEXT("DevicePreviewUnsupported", "No preview for device type: {0}"), StaticEnum<EJoystickType>()->GetDisplayNameTextByValue(static_cast<int64>(PreviewJoystickType)));
				})
				.TextStyle(FAppStyle::Get(), "SmallText")
				.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				.Visibility_Lambda(GetPreviewVisibility)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 8, 0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("RotationRangeLabel", "RANGE \u00B1"))
					.TextStyle(FAppStyle::Get(), "SmallText")
					.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SSpinBox<float>)
					.MinValue(FHapticEffectEditorPreview::MinRotationLimitDegrees)
					.MaxValue(FHapticEffectEditorPreview::MaxRotationLimitDegrees)
					.Delta(10.0f)
					.MaxFractionalDigits(0)
					.ToolTipText(LOCTEXT("RotationRangeTooltip",
					                     "How far the wheel turns each way from centre, in degrees (540 = -540 to +540). The device doesn't report this, so set it to match the range configured in the wheel's own driver software."))
					.Value_Lambda([this] { return PreviewMaxRotationDegrees; })
					.OnValueChanged_Lambda([this](const float NewValue) { PreviewMaxRotationDegrees = NewValue; })
				]
			]
		]
	];
}

void SHapticDevicePreviewPanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdatePreviewAxis();
}

void SHapticDevicePreviewPanel::ResetAxisDetection()
{
	PlayAxisBaselines.Reset();
	PlayAxisDeviations.Reset();
}

void SHapticDevicePreviewPanel::UpdatePreviewAxis()
{
	UJoystickSubsystem* JoystickSubsystem = GEngine ? GEngine->GetEngineSubsystem<UJoystickSubsystem>() : nullptr;
	const TSharedPtr<FJoystickInstanceId> SelectedJoystick = Joystick.Get();
	const bool bHasJoystick = SelectedJoystick.IsValid() && JoystickSubsystem != nullptr;

	const FJoystickInstanceId CurrentJoystickId = bHasJoystick ? *SelectedJoystick : FJoystickInstanceId();
	if (!CurrentJoystickId.Equals(PreviewJoystickId))
	{
		PreviewJoystickId = CurrentJoystickId;
		PreviewJoystickType = bHasJoystick ? JoystickSubsystem->GetJoystickType(CurrentJoystickId) : EJoystickType::Unknown;
		PreviewAxisIndex = 0;
		PlayAxisBaselines.Reset();
		PlayAxisDeviations.Reset();
	}

	PreviewAxisValue = 0.0f;

	FJoystickDeviceState JoystickState;
	if (!bHasJoystick || !JoystickSubsystem->GetJoystickState(CurrentJoystickId, JoystickState))
	{
		return;
	}

	// Raw Value rather than GetValue(), to show where the hardware physically is before any remapping.
	const TArray<FAxisData>& Axes = JoystickState.Axes;

	if (IsPlaying.Get())
	{
		if (PlayAxisBaselines.Num() != Axes.Num())
		{
			PlayAxisBaselines.Reset(Axes.Num());
			for (const FAxisData& Axis : Axes)
			{
				PlayAxisBaselines.Add(Axis.Value);
			}

			PlayAxisDeviations.Init(0.0f, Axes.Num());
		}

		// Tracks each axis's furthest excursion, since a periodic effect swings the driven axis back through its start.
		int32 MostMovedAxisIndex = INDEX_NONE;
		float MostMovedDeviation = FHapticEffectEditorPreview::AxisDetectionThreshold;
		for (int32 AxisIndex = 0; AxisIndex < Axes.Num(); ++AxisIndex)
		{
			PlayAxisDeviations[AxisIndex] = FMath::Max(PlayAxisDeviations[AxisIndex], FMath::Abs(Axes[AxisIndex].Value - PlayAxisBaselines[AxisIndex]));
			if (PlayAxisDeviations[AxisIndex] > MostMovedDeviation)
			{
				MostMovedDeviation = PlayAxisDeviations[AxisIndex];
				MostMovedAxisIndex = AxisIndex;
			}
		}

		if (MostMovedAxisIndex != INDEX_NONE)
		{
			PreviewAxisIndex = MostMovedAxisIndex;
		}
	}

	if (Axes.IsValidIndex(PreviewAxisIndex))
	{
		PreviewAxisValue = Axes[PreviewAxisIndex].Value;
	}
}

int32 SHapticDevicePreviewPanel::GetPreviewAxisIndex() const
{
	return PreviewAxisIndex;
}

float SHapticDevicePreviewPanel::GetPreviewAxisDegrees() const
{
	return PreviewAxisValue * PreviewMaxRotationDegrees;
}

#undef LOCTEXT_NAMESPACE
