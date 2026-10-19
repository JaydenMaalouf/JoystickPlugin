// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#if WITH_EDITOR
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
#include "Misc/DataValidation.h"
#endif
#endif

void UForceFeedbackEffectGroup::GetActiveEntries(TArray<FForceFeedbackEffectGroupEntry>& OutEntries) const
{
	for (const FForceFeedbackEffectGroupEntry& Entry : Entries)
	{
		if (!Entry.Enabled || !IsValid(Entry.Effect))
		{
			continue;
		}

		OutEntries.Add(Entry);
	}

	OutEntries.StableSort([](const FForceFeedbackEffectGroupEntry& A, const FForceFeedbackEffectGroupEntry& B)
	{
		return A.Priority > B.Priority;
	});
}

#if WITH_EDITOR
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
EDataValidationResult UForceFeedbackEffectGroup::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	TSet<FName> Names;
	for (int i = 0; i < Entries.Num(); i++)
	{
		const FForceFeedbackEffectGroupEntry& Entry = Entries[i];

		if (!IsValid(Entry.Effect))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Entry %d has no effect set."), i)));
			Result = EDataValidationResult::Invalid;
		}
		else if (Entry.Effect->GetClass() == UForceFeedbackEffectBase::StaticClass())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Entry %d uses the base effect class, which does not produce any force. Pick a concrete effect type."), i)));
			Result = EDataValidationResult::Invalid;
		}

		if (Entry.Name.IsNone())
		{
			continue;
		}

		bool AlreadyInSet = false;
		Names.Add(Entry.Name, &AlreadyInSet);
		if (AlreadyInSet)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Entry %d reuses the name '%s'. Entry names must be unique within a group."), i, *Entry.Name.ToString())));
			Result = EDataValidationResult::Invalid;
		}
	}

	return Result == EDataValidationResult::Invalid ? Result : EDataValidationResult::Valid;
}
#endif
#endif
