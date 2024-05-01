// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "SJoystickInputSelector.h"

#include "JoystickInputSettings.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

SJoystickInputSelector::SJoystickInputSelector()
	: bIsSelectingKey(true)
	  , MinRangeOffset(0.0f)
	  , MaxRangeOffset(0.0f)
	  , AxisSelectionTimeout(2.5f)
	  , DeadZone(0.0f)
	  , bAllowAxisKeys(true)
	  , bAllowButtonKeys(true)
	  , bAllowModifierKeys(true)
	  , bAllowGamepadKeys(true)
	  , bEscapeCancelsSelection(true)
	  , bIsFocusable(false)
{
}

FReply SJoystickInputSelector::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bIsSelectingKey)
	{
		if (SelectedKey.IsSet() && SelectedKey.Get().Key.IsValid() && (bAllowGamepadKeys && InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Left))
		{
			SetSelectedKey(FInputChord());
			return FReply::Handled();
		}
		if (Button.IsValid())
		{
			return Button->OnKeyDown(MyGeometry, InKeyEvent);
		}
	}
	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

FReply SJoystickInputSelector::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bAllowButtonKeys)
	{
		return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
	}

	const FKey KeyUp = InKeyEvent.GetKey();
	const EModifierKey::Type ModifierKey = EModifierKey::FromBools(
		InKeyEvent.IsControlDown() && KeyUp != EKeys::LeftControl && KeyUp != EKeys::RightControl,
		InKeyEvent.IsAltDown() && KeyUp != EKeys::LeftAlt && KeyUp != EKeys::RightAlt,
		InKeyEvent.IsShiftDown() && KeyUp != EKeys::LeftShift && KeyUp != EKeys::RightShift,
		InKeyEvent.IsCommandDown() && KeyUp != EKeys::LeftCommand && KeyUp != EKeys::RightCommand);


	// Don't allow chords consisting of just modifier keys.
	if (bIsSelectingKey && (bAllowGamepadKeys || KeyUp.IsGamepadKey() == false) && (KeyUp.IsModifierKey() == false || ModifierKey == EModifierKey::None))
	{
		SetIsSelectingKey(false);

		if (bEscapeCancelsSelection && (KeyUp == EKeys::Escape || IsEscapeKey(KeyUp)))
		{
			return FReply::Handled();
		}

		const FInputChord NewSelectedKey = bAllowModifierKeys
			                                   ? FInputChord(KeyUp, ModifierKey == EModifierKey::Shift,
			                                                 ModifierKey == EModifierKey::Control,
			                                                 ModifierKey == EModifierKey::Alt,
			                                                 ModifierKey == EModifierKey::Command)
			                                   : FInputChord(KeyUp);

		SelectKey(NewSelectedKey);
		return FReply::Handled();
	}
	if (!bIsSelectingKey && Button.IsValid())
	{
		return Button->OnKeyUp(MyGeometry, InKeyEvent);
	}

	return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
}

FReply SJoystickInputSelector::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogInputEvent)
{
	/** Don't process events in dead zone */
	const float AbsAnalogValue = FMath::Abs(InAnalogInputEvent.GetAnalogValue());

	if (AbsAnalogValue <= DeadZone)
	{
		return FReply::Handled();
	}
	
	if (!bAllowAxisKeys)
	{
		return SCompoundWidget::OnAnalogValueChanged(MyGeometry, InAnalogInputEvent);
	}

	const FKey AxisKey = InAnalogInputEvent.GetKey();
	const EModifierKey::Type ModifierKey = EModifierKey::FromBools(
		InAnalogInputEvent.IsControlDown() && AxisKey != EKeys::LeftControl && AxisKey != EKeys::RightControl,
		InAnalogInputEvent.IsAltDown() && AxisKey != EKeys::LeftAlt && AxisKey != EKeys::RightAlt,
		InAnalogInputEvent.IsShiftDown() && AxisKey != EKeys::LeftShift && AxisKey != EKeys::RightShift,
		InAnalogInputEvent.IsCommandDown() && AxisKey != EKeys::LeftCommand && AxisKey != EKeys::RightCommand);

	// Don't allow chords consisting of just modifier keys.
	if (bIsSelectingKey && (bAllowGamepadKeys || AxisKey.IsGamepadKey() == false) && (AxisKey.IsModifierKey() == false || ModifierKey == EModifierKey::None))
	{
		if (bEscapeCancelsSelection && (AxisKey == EKeys::Escape || IsEscapeKey(AxisKey)))
		{
			return FReply::Handled();
		}

		const FInputChord NewSelectedKey = bAllowModifierKeys
			                                   ? FInputChord(AxisKey, ModifierKey == EModifierKey::Shift,
			                                                 ModifierKey == EModifierKey::Control,
			                                                 ModifierKey == EModifierKey::Alt,
			                                                 ModifierKey == EModifierKey::Command)
			                                   : FInputChord(AxisKey);


		const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
		if (!IsValid(JoystickInputSettings))
		{
			return FReply::Handled();
		}

		const float AxisValue = InAnalogInputEvent.GetAnalogValue();
		FKeySelectorData& SelectedKeyData = KeyData.FindOrAdd(NewSelectedKey);

		const FJoystickInputDeviceAxisProperties* AxisProperties = JoystickInputSettings->GetAxisPropertiesByKey(AxisKey);
		if (AxisProperties != nullptr)
		{
			if (AxisValue == AxisProperties->OutputRangeMin)
			{
				const int64 CurrentTick = FDateTime::UtcNow().GetTicks();
				SelectedKeyData.MinStartTime = FTimespan(CurrentTick);
			}
			else if (AxisValue == AxisProperties->OutputRangeMax)
			{
				const int64 CurrentTick = FDateTime::UtcNow().GetTicks();
				SelectedKeyData.MaxStartTime = FTimespan(CurrentTick);
			}
		}
		else
		{
			if (AxisValue == 0.0f)
			{
				const int64 CurrentTick = FDateTime::UtcNow().GetTicks();
				SelectedKeyData.MinStartTime = FTimespan(CurrentTick);
			}
			else if (AxisValue == 1.0f)
			{
				const int64 CurrentTick = FDateTime::UtcNow().GetTicks();
				SelectedKeyData.MaxStartTime = FTimespan(CurrentTick);
			}
		}

		const double MinMaxTime = (SelectedKeyData.MinStartTime - SelectedKeyData.MaxStartTime).GetTotalSeconds();
		if (MinMaxTime > 0 && MinMaxTime < AxisSelectionTimeout)
		{
			SetIsSelectingKey(false);
			SelectAxis(NewSelectedKey);
			return FReply::Handled();
		}

		const double MaxMinTime = (SelectedKeyData.MaxStartTime - SelectedKeyData.MinStartTime).GetTotalSeconds();
		if (MaxMinTime > 0 && MaxMinTime < AxisSelectionTimeout)
		{
			SetIsSelectingKey(false);
			SelectAxis(NewSelectedKey);
			return FReply::Handled();
		}
	}
	else if (!bIsSelectingKey && Button.IsValid())
	{
		return Button->OnAnalogValueChanged(MyGeometry, InAnalogInputEvent);
	}

	return SCompoundWidget::OnAnalogValueChanged(MyGeometry, InAnalogInputEvent);
}

void SJoystickInputSelector::Construct(const FArguments& InArgs)
{
	SelectedKey = InArgs._SelectedKey;
	KeySelectionText = InArgs._KeySelectionText;
	NoKeySpecifiedText = InArgs._NoKeySpecifiedText;
	OnAxisSelected = InArgs._OnAxisSelected;
	OnKeySelected = InArgs._OnKeySelected;
	OnIsSelectingChanged = InArgs._OnIsSelectingChanged;
	bAllowAxisKeys = InArgs._AllowAxisKeys;
	bAllowButtonKeys = InArgs._AllowButtonKeys;
	bAllowModifierKeys = InArgs._AllowModifierKeys;
	bAllowGamepadKeys = InArgs._AllowGamepadKeys;
	bEscapeCancelsSelection = InArgs._EscapeCancelsSelection;
	EscapeKeys = InArgs._EscapeKeys;
	bIsFocusable = InArgs._IsFocusable;

	bIsSelectingKey = false;

	ChildSlot
	[
		SAssignNew(Button, SButton)
		.ButtonStyle(InArgs._ButtonStyle)
		.IsFocusable(bIsFocusable)
		.OnClicked(this, &SJoystickInputSelector::OnClicked)
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(this, &SJoystickInputSelector::GetSelectedKeyText)
			.TextStyle(InArgs._TextStyle)
			.Margin(Margin)
			.Justification(ETextJustify::Center)
		]
	];
}

FText SJoystickInputSelector::GetSelectedKeyText() const
{
	if (bIsSelectingKey)
	{
		return KeySelectionText;
	}
	if (SelectedKey.IsSet())
	{
		if (SelectedKey.Get().Key.IsValid())
		{
			// If the key in the chord is a modifier key, print it's display name directly since the FInputChord
			// displays these as empty text.
			return SelectedKey.Get().Key.IsModifierKey()
				       ? SelectedKey.Get().Key.GetDisplayName()
				       : SelectedKey.Get().GetInputText();
		}
	}
	return NoKeySpecifiedText;
}

FInputChord SJoystickInputSelector::GetSelectedKey() const
{
	return SelectedKey.IsSet() ? SelectedKey.Get() : EKeys::Invalid;
}

void SJoystickInputSelector::SetSelectedKey(TAttribute<FInputChord> InSelectedKey)
{
	if (SelectedKey.IdenticalTo(InSelectedKey) == false)
	{
		SelectedKey = InSelectedKey;
		OnAxisSelected.ExecuteIfBound(SelectedKey.IsSet() ? SelectedKey.Get() : FInputChord(EKeys::Invalid));
	}
}

FMargin SJoystickInputSelector::GetMargin() const
{
	return Margin.Get();
}

void SJoystickInputSelector::SetMargin(TAttribute<FMargin> InMargin)
{
	Margin = InMargin;
}

void SJoystickInputSelector::SetButtonStyle(const FButtonStyle* ButtonStyle)
{
	if (Button.IsValid())
	{
		Button->SetButtonStyle(ButtonStyle);
	}
}

void SJoystickInputSelector::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetTextStyle(InTextStyle);
	}
}

FReply SJoystickInputSelector::OnClicked()
{
	if (bIsSelectingKey == false)
	{
		SetIsSelectingKey(true);
		return FReply::Handled().SetUserFocus(SharedThis(this), EFocusCause::SetDirectly);
	}
	return FReply::Handled();
}

void SJoystickInputSelector::SelectKey(FInputChord NewSelectedKey)
{
	if (SelectedKey.IsBound() == false)
	{
		SelectedKey.Set(NewSelectedKey);
	}
	OnKeySelected.ExecuteIfBound(NewSelectedKey);
}

void SJoystickInputSelector::SelectAxis(FInputChord NewSelectedKey)
{
	if (SelectedKey.IsBound() == false)
	{
		SelectedKey.Set(NewSelectedKey);
	}
	OnAxisSelected.ExecuteIfBound(NewSelectedKey);
}

void SJoystickInputSelector::SetIsSelectingKey(bool bInIsSelectingKey)
{
	if (bIsSelectingKey != bInIsSelectingKey)
	{
		bIsSelectingKey = bInIsSelectingKey;
		// Prevents certain inputs from being consumed by the button
		if (Button.IsValid())
		{
			Button->SetEnabled(!bIsSelectingKey);
		}
		OnIsSelectingChanged.ExecuteIfBound();
	}
}

bool SJoystickInputSelector::IsEscapeKey(const FKey& InKey) const
{
	return EscapeKeys.Contains(InKey);
}

FReply SJoystickInputSelector::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsSelectingKey && (bAllowGamepadKeys || InKeyEvent.GetKey().IsGamepadKey() == false))
	{
		// While selecting keys handle all key downs to prevent contained controls from
		// interfering with key selection.
		return FReply::Handled();
	}
	return SCompoundWidget::OnPreviewKeyDown(MyGeometry, InKeyEvent);
}

FReply SJoystickInputSelector::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsSelectingKey)
	{
		SetIsSelectingKey(false);
		// TODO: Add options for enabling mouse modifiers.
		SelectKey(FInputChord(MouseEvent.GetEffectingButton(), false, false, false, false));
		return FReply::Handled();
	}
	return SCompoundWidget::OnPreviewMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SJoystickInputSelector::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsSelectingKey && SelectedKey.IsSet() && SelectedKey.Get().Key.IsValid() && MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		SetSelectedKey(FInputChord());
		return FReply::Handled();
	}
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FNavigationReply SJoystickInputSelector::OnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent)
{
	if (Button.IsValid())
	{
		return Button->OnNavigation(MyGeometry, InNavigationEvent);
	}

	return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
}

void SJoystickInputSelector::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (bIsSelectingKey)
	{
		SetIsSelectingKey(false);
	}
}

void SJoystickInputSelector::SetTextBlockVisibility(EVisibility InVisibility)
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetVisibility(InVisibility);
	}
}
