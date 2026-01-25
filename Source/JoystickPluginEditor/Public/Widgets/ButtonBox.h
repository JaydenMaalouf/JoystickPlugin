// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SButtonBox final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SButtonBox)
		{
		}

		SLATE_ATTRIBUTE(bool, Value)
		SLATE_ATTRIBUTE(int, ButtonIndex)
		SLATE_ATTRIBUTE(FText, DisplayName)
		SLATE_EVENT(FSimpleDelegate, OnClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual FSlateColor GetForegroundColor() const override;

	void SetValue(const bool InValue);

private:
	TAttribute<bool> Value;
	TAttribute<int> ButtonIndex;
	TAttribute<FText> DisplayName;
	FSimpleDelegate OnClickedDelegate;

	FText ButtonText;

	FLinearColor GetColor() const;
	FSlateColor GetBackgroundColor() const;
	FReply HandleClicked();
};
