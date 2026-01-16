// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"

class SAxisBar;

class JOYSTICKPLUGINEDITOR_API SBaseKeyConfigurationEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseKeyConfigurationEditor)
		{
		}

		SLATE_ARGUMENT(FKey, Key)
		SLATE_ARGUMENT(FJoystickInstanceId, InstanceId)
		SLATE_ARGUMENT(int, Index)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SBaseKeyConfigurationEditor() override;

	virtual void LoadConfiguration();
	virtual void SaveConfiguration() const;

protected:
	FKey Key;
	FJoystickInstanceId InstanceId;
	int KeyIndex = -1;

	virtual void ShowRestartRequiredNotification() const;
	virtual FReply OnSaveClicked();
	virtual FReply OnCancelClicked();

	// Returns the Save/Cancel button widget to be added at the bottom of the editor
	TSharedRef<SWidget> MakeActionButtons();
};
