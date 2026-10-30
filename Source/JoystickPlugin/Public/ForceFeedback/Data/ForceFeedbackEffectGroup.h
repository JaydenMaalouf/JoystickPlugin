// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Runtime/Launch/Resources/Version.h"

#include "ForceFeedbackEffectGroup.generated.h"

class UForceFeedbackEffectBase;
class FDataValidationContext;

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectGroupEntry
{
	GENERATED_BODY()

	FForceFeedbackEffectGroupEntry()
		: Effect(nullptr)
		  , Enabled(true)
		  , Priority(0)
		  , AutoStart(true)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Force Feedback", meta=(ToolTip="The effect to spawn for each supported device, tuned for this group."))
	UForceFeedbackEffectBase* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Disabled entries are not spawned."))
	bool Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Optional name used to find, start or stop this effect on the component at runtime."))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback",
		meta=(ToolTip="Optional tag used to find, start or stop this effect on the component at runtime. Triggering a parent tag also triggers this effect, and several entries can share a tag."))
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback",
		meta=(ToolTip="Higher priority effects are created first. When a device runs out of effect slots, the lowest priority effects are skipped. Compared across every group on the component."))
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Start this effect whenever the component starts. Disable for effects that are triggered by name or tag, such as one-shot impacts."))
	bool AutoStart;
};

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UForceFeedbackEffectGroup : public UDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
#endif

	// Returns the enabled entries that have an effect set, highest priority first.
	void GetActiveEntries(TArray<FForceFeedbackEffectGroupEntry>& OutEntries) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Identifies this group, so it can be matched against gameplay state such as a surface type."))
	FGameplayTag GroupTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(TitleProperty="Name"))
	TArray<FForceFeedbackEffectGroupEntry> Entries;
};
