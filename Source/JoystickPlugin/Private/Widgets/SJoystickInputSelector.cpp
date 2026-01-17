// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/SJoystickInputSelector.h"

#include "JoystickFunctionLibrary.h"
#include "JoystickInputSettings.h"
#include "Data/InputSelectorTypes.h"
#include "Data/KeySelectorTypes.h"
#include "Managers/JoystickProfileManager.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Input/SButton.h"

void SJoystickInputSelector::Construct(const FArguments& InArgs)
{
	SelectedKey = InArgs._SelectedKey;
	KeySelectionText = InArgs._KeySelectionText;
	NoKeySpecifiedText = InArgs._NoKeySpecifiedText;
	OnKeySelected = InArgs._OnKeySelected;
	OnIsSelectingChanged = InArgs._OnIsSelectingChanged;
	InputSelectorTypes = InArgs._InputSelectorTypes;
	KeySelectorTypes = InArgs._KeySelectorTypes;
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
			SNew(SOverlay)
			.Visibility(EVisibility::SelfHitTestInvisible)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(TextBlock, STextBlock)
				.Text(this, &SJoystickInputSelector::GetSelectedKeyText)
				.TextStyle(InArgs._TextStyle)
				.Margin(Margin)
				.Justification(ETextJustify::Center)
			]
		]
	];
}

FInputChord SJoystickInputSelector::GetSelectedKey() const
{
	return SelectedKey.IsSet() ? SelectedKey.Get() : EKeys::Invalid;
}

void SJoystickInputSelector::SetSelectedKey(const TAttribute<FInputChord>& InSelectedKey)
{
	if (SelectedKey.IdenticalTo(InSelectedKey) == false)
	{
		SelectedKey = InSelectedKey;
		OnKeySelected.ExecuteIfBound(SelectedKey.IsSet() ? SelectedKey.Get() : FInputChord(EKeys::Invalid));
	}
}

void SJoystickInputSelector::SetMargin(const TAttribute<FMargin>& InMargin)
{
	Margin = InMargin;
}

void SJoystickInputSelector::SetButtonStyle(const FButtonStyle* ButtonStyle) const
{
	if (Button.IsValid())
	{
		Button->SetButtonStyle(ButtonStyle);
	}
}

void SJoystickInputSelector::SetTextStyle(const FTextBlockStyle* InTextStyle) const
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetTextStyle(InTextStyle);
	}
}

void SJoystickInputSelector::SetInputSelectorTypes(const int32 NewInputSelectorTypes)
{
	InputSelectorTypes = NewInputSelectorTypes;
}

void SJoystickInputSelector::SetKeySelectorTypes(const int32 NewKeySelectorTypes)
{
	KeySelectorTypes = NewKeySelectorTypes;
}

void SJoystickInputSelector::SetTextBlockVisibility(const EVisibility InVisibility) const
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetVisibility(InVisibility);
	}
}

FReply SJoystickInputSelector::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogInputEvent)
{
	const bool bAllowAxisKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Axis)) != 0;
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

	const bool bAllowModifierKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Modifier)) != 0;
	const FInputChord NewSelectedKey = bAllowModifierKeys
		                                   ? FInputChord(AxisKey,
		                                                 ModifierKey == EModifierKey::Shift,
		                                                 ModifierKey == EModifierKey::Control,
		                                                 ModifierKey == EModifierKey::Alt,
		                                                 ModifierKey == EModifierKey::Command)
		                                   : FInputChord(AxisKey);

	FKeySelectorData& SelectedKeyData = KeyData.FindOrAdd(NewSelectedKey.Key);

	// Skip if it's a modifier key and either modifiers aren't allowed, or other modifiers are pressed
	const bool bShouldSkipModifierKey = AxisKey.IsModifierKey() && (!bAllowModifierKeys || ModifierKey != EModifierKey::None);

	if (bIsSelectingKey &&
		ShouldProcessInputKey(AxisKey) &&
		!bShouldSkipModifierKey)
	{
		if (bEscapeCancelsSelection && (AxisKey == EKeys::Escape || IsEscapeKey(AxisKey)))
		{
			return FReply::Handled();
		}

		UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
		if (!IsValid(JoystickProfileManager))
		{
			return FReply::Handled();
		}

		const float AxisValue = InAnalogInputEvent.GetAnalogValue();

		float MinRangeDetection = MinRange;
		float MaxRangeDetection = MaxRange;

		const FJoystickInputDeviceAxisProperties* AxisProperties = JoystickProfileManager->GetAxisPropertiesByKey(AxisKey);
		if (UseAxisProperties && AxisProperties != nullptr)
		{
			MinRangeDetection = AxisProperties->OutputRangeMin;
			MaxRangeDetection = AxisProperties->OutputRangeMax;
		}

		const float PhysicalMin = MinRangeDetection;
		const float PhysicalMax = MaxRangeDetection;

		// Clamp targets so they are always reachable
		const float TargetMin = FMath::Clamp(PhysicalMin + MinRangeOffset, PhysicalMin, PhysicalMax);
		const float TargetMax = FMath::Clamp(PhysicalMax + MaxRangeOffset, PhysicalMin, PhysicalMax);

		const float Tolerance = FMath::Max(DeadZone, 0.0025f);

		const bool bAtMin = AxisValue <= TargetMin + Tolerance;
		const bool bAtMax = AxisValue >= TargetMax - Tolerance;

		const double Now = FPlatformTime::Seconds();

		if (!SelectedKeyData.bWasSelecting)
		{
			SelectedKeyData.bWasSelecting = true;
			SelectedKeyData.Reset();
		}

		if ((SelectedKeyData.State == EAxisSelectState::FromMin || SelectedKeyData.State == EAxisSelectState::FromMax) &&
			(Now - SelectedKeyData.StartTimeSeconds) > AxisSelectionTimeout)
		{
			SelectedKeyData.Reset();
			return FReply::Handled();
		}

		const bool bHitMinEdge = bAtMin && !SelectedKeyData.bPrevAtMin;
		const bool bHitMaxEdge = bAtMax && !SelectedKeyData.bPrevAtMax;

		switch (SelectedKeyData.State)
		{
		case EAxisSelectState::Idle:
			{
				if (bHitMinEdge)
				{
					SelectedKeyData.State = EAxisSelectState::FromMin;
					SelectedKeyData.StartTimeSeconds = Now;
				}
				else if (bHitMaxEdge)
				{
					SelectedKeyData.State = EAxisSelectState::FromMax;
					SelectedKeyData.StartTimeSeconds = Now;
				}
				break;
			}

		case EAxisSelectState::FromMin:
			{
				if (bAtMax)
				{
					SelectedKeyData.Reset();
					SelectedKeyData.bWasSelecting = true;
					SetIsSelectingKey(false);
					SelectKey(
						AxisKey,
						ModifierKey == EModifierKey::Shift,
						ModifierKey == EModifierKey::Control,
						ModifierKey == EModifierKey::Alt,
						ModifierKey == EModifierKey::Command);
					return FReply::Handled();
				}
				break;
			}

		case EAxisSelectState::FromMax:
			{
				if (bAtMin)
				{
					SelectedKeyData.Reset();
					SelectedKeyData.bWasSelecting = true;
					SetIsSelectingKey(false);
					SelectKey(
						AxisKey,
						ModifierKey == EModifierKey::Shift,
						ModifierKey == EModifierKey::Control,
						ModifierKey == EModifierKey::Alt,
						ModifierKey == EModifierKey::Command);
					return FReply::Handled();
				}
				break;
			}
		}

		// Update edge history AFTER processing
		SelectedKeyData.bPrevAtMin = bAtMin;
		SelectedKeyData.bPrevAtMax = bAtMax;

		return FReply::Handled();
	}

	if (!bIsSelectingKey && Button.IsValid())
	{
		SelectedKeyData.Reset();
		return FReply::Handled();
	}

	return SCompoundWidget::OnAnalogValueChanged(MyGeometry, InAnalogInputEvent);
}

FReply SJoystickInputSelector::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bIsSelectingKey)
	{
		if (SelectedKey.IsSet() && SelectedKey.Get().Key.IsValid() && ShouldProcessInputKey(InKeyEvent.GetKey()))
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
	const bool bAllowButtonKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Button)) != 0;
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

	const bool bAllowModifierKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Modifier)) != 0;

	// Skip if it's a modifier key and either modifiers aren't allowed, or other modifiers are pressed
	// (Don't allow chords consisting of just modifier keys)
	const bool bShouldSkipModifierKey = KeyUp.IsModifierKey() && (!bAllowModifierKeys || ModifierKey != EModifierKey::None);

	if (bIsSelectingKey &&
		ShouldProcessInputKey(KeyUp) &&
		!bShouldSkipModifierKey)
	{
		SetIsSelectingKey(false);
		if (bEscapeCancelsSelection && (KeyUp == EKeys::Escape || IsEscapeKey(KeyUp)))
		{
			return FReply::Handled();
		}

		SelectKey(
			KeyUp,
			ModifierKey == EModifierKey::Shift,
			ModifierKey == EModifierKey::Control,
			ModifierKey == EModifierKey::Alt,
			ModifierKey == EModifierKey::Command);
		return FReply::Handled();
	}
	if (!bIsSelectingKey && Button.IsValid())
	{
		return Button->OnKeyUp(MyGeometry, InKeyEvent);
	}

	return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
}

FReply SJoystickInputSelector::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsSelectingKey && ShouldProcessInputKey(InKeyEvent.GetKey()))
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
		const FKey MouseButton = MouseEvent.GetEffectingButton();
		if (ShouldProcessInputKey(MouseButton))
		{
			SetIsSelectingKey(false);
			// TODO: Add options for enabling mouse modifiers.
			SelectKey(MouseButton, false, false, false, false);
			return FReply::Handled();
		}
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

void SJoystickInputSelector::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (bIsSelectingKey)
	{
		SetIsSelectingKey(false);
	}
}

FNavigationReply SJoystickInputSelector::OnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent)
{
	if (Button.IsValid())
	{
		return Button->OnNavigation(MyGeometry, InNavigationEvent);
	}

	return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
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

FMargin SJoystickInputSelector::GetMargin() const
{
	return Margin.Get();
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

void SJoystickInputSelector::SelectKey(const FKey& Key, const bool bShiftDown, const bool bControlDown, const bool bAltDown, const bool bCommandDown)
{
	const bool bAllowModifierKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Modifier)) != 0;
	const FInputChord NewSelectedKey = bAllowModifierKeys
		                                   ? FInputChord(Key, bShiftDown, bControlDown, bAltDown, bCommandDown)
		                                   : FInputChord(Key);
	if (SelectedKey.IsBound() == false)
	{
		SelectedKey.Set(NewSelectedKey);
	}
	OnKeySelected.ExecuteIfBound(NewSelectedKey);
}

void SJoystickInputSelector::SetIsSelectingKey(const bool bInIsSelectingKey)
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

bool SJoystickInputSelector::ShouldProcessInputKey(const FKey& InKey) const
{
	const bool bAllowKeyboardKeys = (InputSelectorTypes & static_cast<int32>(EInputSelectorTypes::Keyboard)) != 0;
	const bool bAllowMouseKeys = (InputSelectorTypes & static_cast<int32>(EInputSelectorTypes::Mouse)) != 0;
	const bool bAllowGamepadKeys = (InputSelectorTypes & static_cast<int32>(EInputSelectorTypes::Gamepad)) != 0;
	const bool bAllowJoystickKeys = (InputSelectorTypes & static_cast<int32>(EInputSelectorTypes::Joystick)) != 0;

	const bool bAllowAxisKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Axis)) != 0;
	const bool bAllowButtonKeys = (KeySelectorTypes & static_cast<int32>(EKeySelectorTypes::Button)) != 0;

	// Check if key type (Axis/Button) is allowed
	// Modifier keys are handled separately in the calling functions, so skip them here
	const bool bIsAxisKey = InKey.IsAnalog();
	const bool bIsButtonKey = InKey.IsDigital();

	// If it's an axis key, check if axes are allowed
	if (bIsAxisKey && !bAllowAxisKeys)
	{
		return false;
	}

	if (bIsButtonKey && !bAllowButtonKeys)
	{
		return false;
	}

	// Check joystick keys FIRST (since they are a subset of gamepad keys)
	// This allows joystick keys to be evaluated separately from regular gamepad keys
	if (UJoystickFunctionLibrary::IsJoystickKey(InKey))
	{
		return bAllowJoystickKeys;
	}

	// Check keyboard keys
	if (InKey.IsValid() && !InKey.IsGamepadKey() && !InKey.IsMouseButton())
	{
		return bAllowKeyboardKeys;
	}

	// Check mouse keys
	if (InKey.IsMouseButton())
	{
		return bAllowMouseKeys;
	}

	// Check gamepad keys (after joystick check, so regular gamepad keys are handled here)
	if (InKey.IsGamepadKey())
	{
		return bAllowGamepadKeys;
	}

	// Default: allow if no specific type matched (shouldn't happen, but be permissive)
	return true;
}
