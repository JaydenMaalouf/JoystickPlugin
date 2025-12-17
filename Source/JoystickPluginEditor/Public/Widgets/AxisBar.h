// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAxisBar final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAxisBar)
		{
		}

		SLATE_ARGUMENT(INT, AxisIndex)
		SLATE_ARGUMENT(FText, DisplayName)
		SLATE_ARGUMENT(float, Value)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetValue(const float InValue);

private:
	TAttribute<int> AxisIndex;
	TAttribute<FText> DisplayName;
	TAttribute<float> Value; // Ranges from -1 to 1

	FText ButtonText;
};
