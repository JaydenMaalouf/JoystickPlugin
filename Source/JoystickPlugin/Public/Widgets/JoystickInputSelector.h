// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Framework/Commands/InputChord.h"
#include "Fonts/SlateFontInfo.h"
#include "Layout/Margin.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "Styling/SlateTypes.h"
#include "Layout/Visibility.h"

#include "JoystickInputSelector.generated.h"

class SJoystickInputSelector;
struct FButtonStyle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAxisSelected, FInputChord, SelectedAxis);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeySelected, FInputChord, SelectedKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIsSelectingChanged);

/** A widget for selecting a single key or a single key with a modifier. */
UCLASS()
class JOYSTICKPLUGIN_API UJoystickInputSelector : public UWidget
{
	GENERATED_BODY()

public:
	UJoystickInputSelector(const FObjectInitializer& ObjectInitializer);

	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (DisplayName = "Style"))
	FButtonStyle WidgetStyle;

	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (DisplayName = "Text Style"))
	FTextBlockStyle TextStyle;

	/** The currently selected key chord. */
	UPROPERTY(BlueprintReadOnly, Category = "Key Selection")
	FInputChord SelectedKey;

	UPROPERTY()
	FSlateFontInfo Font_DEPRECATED;

	/** The amount of blank space around the text used to display the currently selected key. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FMargin Margin;

	UPROPERTY()
	FLinearColor ColorAndOpacity_DEPRECATED;

	/** Sets the text which is displayed while selecting keys. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FText KeySelectionText;

	/** Sets the text to display when no key text is available or not selecting a key. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FText NoKeySpecifiedText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	bool bAllowButtonKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	bool bAllowAxisKeys;

	/** When true modifier keys such as control and alt are allowed in the */
	/** input chord representing the selected key, if false modifier keys are ignored. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	bool bAllowModifierKeys;

	/** When true gamepad keys are allowed in the input chord representing the selected key, otherwise they are ignored. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	bool bAllowGamepadKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	float MinRangeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	float MaxRangeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	float AxisSelectionTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	float DeadZone;

	/** When true gamepad keys are allowed in the input chord representing the selected key, otherwise they are ignored. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Selection")
	TArray<FKey> EscapeKeys;

	/** Called whenever a new key is selected by the user. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnAxisSelected OnAxisSelected;

	/** Called whenever a new key is selected by the user. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnKeySelected OnKeySelected;

	/** Called whenever the key selection mode starts or stops. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnIsSelectingChanged OnIsSelectingChanged;

	/** Sets the currently selected key. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetSelectedKey(const FInputChord& InSelectedKey);

	/** Sets the text which is displayed while selecting keys. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetKeySelectionText(FText InKeySelectionText);

	/** Sets the text to display when no key text is available or not selecting a key. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetNoKeySpecifiedText(FText InNoKeySpecifiedText);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetAllowAxisKeys(bool bInAllowAxisKeys);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetAllowButtonKeys(bool bInAllowButtonKeys);

	/** Sets whether or not modifier keys are allowed in the selected key. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetAllowModifierKeys(bool bInAllowModifierKeys);

	/** Sets whether or not gamepad keys are allowed in the selected key. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetAllowGamepadKeys(bool bInAllowGamepadKeys);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetMinRangeOffset(float InMinRangeOffset);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetMaxRangeOffset(float InMaxRangeOffset);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetAxisSelectionTimeout(float InAxisSelectionTimeout);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetDeadZone(float InDeadZone);

	/** Returns true if the widget is currently selecting a key, otherwise returns false. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool GetIsSelectingKey() const;

	/** Sets the visibility of the text block. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetTextBlockVisibility(const ESlateVisibility InVisibility);

	/** Sets the style of the button used to start key selection mode. */
	void SetButtonStyle(const FButtonStyle* ButtonStyle);

	/** Sets escape keys. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetEscapeKeys(const TArray<FKey>& InKeys);

	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	//~ Begin UObject Interface
	virtual void PostLoad() override;
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

private:
	virtual void HandleAxisSelected(const FInputChord& InSelectedKey);
	virtual void HandleKeySelected(const FInputChord& InSelectedKey);
	void HandleIsSelectingChanged();

	/** The input key selector widget managed by this object. */
	TSharedPtr<SJoystickInputSelector> JoystickInputSelector;
};
