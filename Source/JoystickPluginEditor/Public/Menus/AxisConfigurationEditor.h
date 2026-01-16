// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "BaseKeyConfigurationEditor.h"
#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"

class SAxisBar;

class JOYSTICKPLUGINEDITOR_API SAxisConfigurationEditor final : public SBaseKeyConfigurationEditor
{
public:
	SLATE_BEGIN_ARGS(SAxisConfigurationEditor)
		{
		}

		SLATE_ARGUMENT(FKey, AxisKey)
		SLATE_ARGUMENT(FJoystickInstanceId, InstanceId)
		SLATE_ARGUMENT(int32, AxisIndex)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SAxisConfigurationEditor() override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	FJoystickInputDeviceAxisProperties CurrentProperties;
	TSharedPtr<STextBlock> CurrentValueText;
	TSharedPtr<SAxisBar> InputVisualiser;

	// Working range radio buttons
	TSharedPtr<SCheckBox> InputRangeNeg1To0Radio;
	TSharedPtr<SCheckBox> InputRangeNeg1To1Radio;
	TSharedPtr<SCheckBox> InputRange0To1Radio;
	int32 SelectedInputRange; // 0: -1 to 0, 1: -1 to 1, 2: 0 to 1

	// Simple property editors
	TSharedPtr<SCheckBox> InvertInputCheckBox;
	TSharedPtr<SCheckBox> OutputRange0To1Radio;
	TSharedPtr<SCheckBox> OutputRangeNeg1To1Radio;
	int32 SelectedOutputRange; // true: 0 to 1, false: -1 to 1

	// Display name override
	TSharedPtr<SCheckBox> OverrideDisplayNameCheckBox;
	TSharedPtr<class SEditableTextBox> DisplayNameTextBox;
	TSharedPtr<class SHorizontalBox> DisplayNameInputContainer;
	FProgressBarStyle ProgressBarStyle;
	
	// Track original display name state for restart notification
	FString OriginalDisplayName;
	bool bOriginalOverrideDisplayName;

	virtual void LoadConfiguration() override;
	virtual void SaveConfiguration() const override;

	void ApplyWorkingRange(); // 0: -1 to 0, 1: -1 to 1, 2: 0 to 1
	void ApplyOutputRange(); // true: 0 to 1, false: -1 to 1
	void UpdateInputRangeButtons() const;
	void UpdateOutputRangeButtons() const;
};
