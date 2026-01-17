// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/JoystickInputSelector.h"

#include "Data/InputSelectorTypes.h"
#include "Data/KeySelectorTypes.h"
#include "Widgets/SJoystickInputSelector.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/FrameworkObjectVersion.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Internationalization/Internationalization.h"
#include "Styling/DefaultStyleCache.h"

#define LOCTEXT_NAMESPACE "JoystickPlugin"

UJoystickInputSelector::UJoystickInputSelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetButtonStyle();
	TextStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetTextBlockStyle();

#if WITH_EDITOR
	if (IsEditorWidget())
	{
		WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetButtonStyle();
		TextStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetTextBlockStyle();

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif // WITH_EDITOR

	KeySelectionText = NSLOCTEXT("InputKeySelector", "DefaultKeySelectionText", "...");
	NoKeySpecifiedText = NSLOCTEXT("InputKeySelector", "DefaultEmptyText", "Empty");
	SelectedKey = FInputChord(EKeys::Invalid);
	AllowedInputTypes = static_cast<int32>(EInputSelectorTypes::Gamepad) | static_cast<int32>(EInputSelectorTypes::Joystick) | static_cast<int32>(EInputSelectorTypes::Keyboard);
	AllowedKeyTypes = static_cast<int32>(EKeySelectorTypes::Axis) | static_cast<int32>(EKeySelectorTypes::Button);
	bUseAxisProperties = true;
	MinRange = 0.0f;
	MaxRange = 1.0f;
	MinRangeOffset = 0.0f;
	MaxRangeOffset = 0.0f;
	DeadZone = 0.05f;
	AxisSelectionTimeout = 5.0f;

	EscapeKeys.AddUnique(EKeys::Escape);
	EscapeKeys.AddUnique(EKeys::Gamepad_Special_Right);
	// In most (if not all) cases this is going to be the menu button

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*GetDefaultFontName());

		TextStyle.Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}
}

void UJoystickInputSelector::SetSelectedKey(const FInputChord& InSelectedKey)
{
	if (SelectedKey != InSelectedKey)
	{
		BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedKey);
		if (JoystickInputSelector.IsValid())
		{
			JoystickInputSelector->SetSelectedKey(InSelectedKey);
		}
		SelectedKey = InSelectedKey;
	}
}


void UJoystickInputSelector::SetKeySelectionText(FText InKeySelectionText)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetKeySelectionText(InKeySelectionText);
	}

	KeySelectionText = MoveTemp(InKeySelectionText);
}

void UJoystickInputSelector::SetNoKeySpecifiedText(FText InNoKeySpecifiedText)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetNoKeySpecifiedText(InNoKeySpecifiedText);
	}

	NoKeySpecifiedText = MoveTemp(InNoKeySpecifiedText);
}

void UJoystickInputSelector::SetUseAxisProperties(const bool bInUseAxisProperties)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetUseAxisProperties(bInUseAxisProperties);
	}

	bUseAxisProperties = bInUseAxisProperties;
}

void UJoystickInputSelector::SetInputSelectionType(const int32 NewMask)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetInputSelectorTypes(NewMask);
	}

	AllowedInputTypes = NewMask;
}

void UJoystickInputSelector::SetKeySelectionType(const int32 NewMask)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetKeySelectorTypes(NewMask);
	}

	AllowedKeyTypes = NewMask;
}

void UJoystickInputSelector::SetMinRange(const float InMinRange)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMinRange(InMinRange);
	}

	MinRange = InMinRange;
}

void UJoystickInputSelector::SetMaxRange(const float InMaxRange)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMaxRange(InMaxRange);
	}

	MaxRange = InMaxRange;
}

void UJoystickInputSelector::SetMinRangeOffset(const float InMinRangeOffset)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMinRangeOffset(InMinRangeOffset);
	}

	MinRangeOffset = InMinRangeOffset;
}

void UJoystickInputSelector::SetMaxRangeOffset(const float InMaxRangeOffset)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMaxRangeOffset(InMaxRangeOffset);
	}

	MaxRangeOffset = InMaxRangeOffset;
}

void UJoystickInputSelector::SetAxisSelectionTimeout(const float InAxisSelectionTimeout)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAxisSelectionTimeout(InAxisSelectionTimeout);
	}

	AxisSelectionTimeout = InAxisSelectionTimeout;
}

void UJoystickInputSelector::SetDeadZone(const float InDeadZone)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetDeadZone(InDeadZone);
	}

	DeadZone = InDeadZone;
}

bool UJoystickInputSelector::GetIsSelectingKey() const
{
	return JoystickInputSelector.IsValid() ? JoystickInputSelector->GetIsSelectingKey() : false;
}

void UJoystickInputSelector::SetButtonStyle(const FButtonStyle& InButtonStyle)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetButtonStyle(&InButtonStyle);
	}
	WidgetStyle = InButtonStyle;
}

void UJoystickInputSelector::SetTextStyle(const FTextBlockStyle& InTextStyle)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetTextStyle(&InTextStyle);
	}
	TextStyle = InTextStyle;
}

void UJoystickInputSelector::SetMargin(const FMargin& InMargin)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMargin(InMargin);
	}
	Margin = InMargin;
}

void UJoystickInputSelector::SetEscapeKeys(const TArray<FKey>& InKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetEscapeKeys(InKeys);
	}
	EscapeKeys = InKeys;
}

void UJoystickInputSelector::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FFrameworkObjectVersion::GUID);
}

#if WITH_EDITOR
const FText UJoystickInputSelector::GetPaletteCategory()
{
	return LOCTEXT("Advanced", "Advanced");
}
#endif

void UJoystickInputSelector::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!JoystickInputSelector.IsValid())
	{
		return;
	}

	JoystickInputSelector->SetSelectedKey(SelectedKey);
	JoystickInputSelector->SetMargin(Margin);
	JoystickInputSelector->SetButtonStyle(&WidgetStyle);
	JoystickInputSelector->SetTextStyle(&TextStyle);
	JoystickInputSelector->SetKeySelectionText(KeySelectionText);
	JoystickInputSelector->SetNoKeySpecifiedText(NoKeySpecifiedText);
	JoystickInputSelector->SetInputSelectorTypes(AllowedInputTypes);
	JoystickInputSelector->SetKeySelectorTypes(AllowedKeyTypes);
	JoystickInputSelector->SetUseAxisProperties(bUseAxisProperties);
	JoystickInputSelector->SetMinRange(MinRange);
	JoystickInputSelector->SetMaxRange(MaxRange);
	JoystickInputSelector->SetMinRangeOffset(MinRangeOffset);
	JoystickInputSelector->SetMaxRangeOffset(MaxRangeOffset);
	JoystickInputSelector->SetAxisSelectionTimeout(AxisSelectionTimeout);
	JoystickInputSelector->SetDeadZone(DeadZone);
	JoystickInputSelector->SetEscapeKeys(EscapeKeys);
}

TSharedRef<SWidget> UJoystickInputSelector::RebuildWidget()
{
	JoystickInputSelector = SNew(SJoystickInputSelector)
		.SelectedKey(SelectedKey)
		.Margin(Margin)
		.ButtonStyle(&WidgetStyle)
		.TextStyle(&TextStyle)
		.KeySelectionText(KeySelectionText)
		.NoKeySpecifiedText(NoKeySpecifiedText)
		.InputSelectorTypes(AllowedInputTypes)
		.KeySelectorTypes(AllowedKeyTypes)
		.SetUseAxisProperties(bUseAxisProperties)
		.SetMinRange(MinRange)
		.SetMaxRange(MaxRange)
		.SetMinRangeOffset(MinRangeOffset)
		.SetMaxRangeOffset(MaxRangeOffset)
		.SetAxisSelectionTimeout(AxisSelectionTimeout)
		.SetDeadZone(DeadZone)
		.EscapeKeys(EscapeKeys)
		.OnKeySelected(BIND_UOBJECT_DELEGATE(SJoystickInputSelector::FOnKeySelected, HandleKeySelected))
		.OnIsSelectingChanged(BIND_UOBJECT_DELEGATE(SJoystickInputSelector::FOnIsSelectingChanged, HandleIsSelectingChanged));
	return JoystickInputSelector.ToSharedRef();
}

void UJoystickInputSelector::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	JoystickInputSelector.Reset();
}

void UJoystickInputSelector::HandleKeySelected(const FInputChord& InSelectedKey)
{
	SelectedKey = InSelectedKey;
	BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedKey);
	OnKeySelected.Broadcast(SelectedKey);
}

void UJoystickInputSelector::SetTextBlockVisibility(const ESlateVisibility InVisibility) const
{
	if (JoystickInputSelector.IsValid())
	{
		const EVisibility SlateVisibility = ConvertSerializedVisibilityToRuntime(InVisibility);
		JoystickInputSelector->SetTextBlockVisibility(SlateVisibility);
	}
}

void UJoystickInputSelector::HandleIsSelectingChanged() const
{
	if (OnIsSelectingChanged.IsBound())
	{
		OnIsSelectingChanged.Broadcast();
	}
}

#undef LOCTEXT_NAMESPACE
