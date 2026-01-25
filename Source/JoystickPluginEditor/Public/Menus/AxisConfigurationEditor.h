// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "BaseKeyConfigurationEditor.h"
#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"

class SAxisBar;
template <typename NumericType>
class SNumericEntryBox;

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

	TSharedPtr<SCheckBox> InputRangeDisabled;
	TSharedPtr<SCheckBox> InputRangeNeg1To0Radio;
	TSharedPtr<SCheckBox> InputRangeNeg1To1Radio;
	TSharedPtr<SCheckBox> InputRange0To1Radio;
	TSharedPtr<SCheckBox> InputRangeCustomRadio;
	int SelectedInputRange = -1;

	TSharedPtr<SNumericEntryBox<float>> CustomRangeMinInput;
	TSharedPtr<SNumericEntryBox<float>> CustomRangeMaxInput;
	TSharedPtr<SButton> SetMinButton;
	TSharedPtr<SButton> SetMaxButton;

	TSharedPtr<SCheckBox> InvertInputCheckBox;
	TSharedPtr<SCheckBox> OutputRangeDisabled;
	TSharedPtr<SCheckBox> OutputRange0To1Radio;
	TSharedPtr<SCheckBox> OutputRangeNeg1To1Radio;
	int SelectedOutputRange = -1;

	TSharedPtr<SCheckBox> OverrideDisplayNameCheckBox;
	TSharedPtr<SEditableTextBox> DisplayNameTextBox;
	TSharedPtr<SHorizontalBox> DisplayNameInputContainer;
	FProgressBarStyle ProgressBarStyle;

	// Track original display name state for restart notification
	FString OriginalDisplayName;
	bool bOriginalOverrideDisplayName = false;

	virtual void LoadConfiguration() override;
	virtual void SaveConfiguration() const override;

	void ApplyWorkingRange();
	void ApplyOutputRange();
	void UpdateInputRangeButtons() const;
	void UpdateOutputRangeButtons() const;
	float GetCurrentAxisValue() const;
	FReply OnSetMinButtonClicked();
	FReply OnSetMaxButtonClicked();
};
