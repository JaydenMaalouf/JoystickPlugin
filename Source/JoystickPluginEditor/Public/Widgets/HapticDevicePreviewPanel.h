// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/JoystickType.h"
#include "Widgets/SCompoundWidget.h"

// The Haptic Effect Editor's device preview section: an SHapticDevicePreview posed from the selected
// joystick's live axis reading, a readout of that axis, and the user-entered rotation range the
// reading is spread across. Works out by itself which axis an effect is driving, by watching which
// one moves the most while IsPlaying is set.
class SHapticDevicePreviewPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticDevicePreviewPanel)
			: _IsPlaying(false)
		{
		}

		// The joystick to preview, null for none.
		SLATE_ATTRIBUTE(TSharedPtr<FJoystickInstanceId>, Joystick)
		SLATE_ATTRIBUTE(bool, IsPlaying)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;

	// Starts axis detection afresh, for a new playback: the baselines are re-captured on the next Tick.
	void ResetAxisDetection();

private:
	void UpdatePreviewAxis();
	float GetPreviewAxisDegrees() const;

	TAttribute<TSharedPtr<FJoystickInstanceId>> Joystick;
	TAttribute<bool> IsPlaying;

	// PreviewJoystickId is the device the cached type/axis below belong to, so a change of selection
	// can be spotted from Tick however it happened (picker, plug, unplug).
	FJoystickInstanceId PreviewJoystickId;
	EJoystickType PreviewJoystickType = EJoystickType::Unknown;
	int32 PreviewAxisIndex = 0;
	float PreviewAxisValue = 0.0f;
	float PreviewMaxRotationDegrees = 540.0f;

	// Per-axis readings taken when playback started and the furthest each axis has strayed from them
	// since, used to work out which axis the effect is physically driving.
	TArray<float> PlayAxisBaselines;
	TArray<float> PlayAxisDeviations;
};
