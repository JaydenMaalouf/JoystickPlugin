// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/HapticEffectEditorProperties.h"

#include "AssetRegistry/AssetData.h"
#include "Engine/Blueprint.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"
#include "Misc/PackageName.h"
#include "UObject/UnrealType.h"

bool FHapticEffectEditorProperties::IsManagedByEditor(const FProperty& Property)
{
	static const TSet<FName> ManagedPropertyNames = {
		GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, InstanceId),
		GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, Configuration),
		GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, Tickable),
		GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, TickableInEditor),
		GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, TickableWhenPaused),
	};

	return ManagedPropertyNames.Contains(Property.GetFName()) || Property.HasAnyPropertyFlags(CPF_EditConst);
}

void FHapticEffectEditorProperties::CopyAuthoredProperties(const UForceFeedbackEffectBase* From, UForceFeedbackEffectBase* To, const UClass* EffectClass)
{
	for (TFieldIterator<FProperty> PropertyIt(EffectClass); PropertyIt; ++PropertyIt)
	{
		const FProperty* Property = *PropertyIt;
		if (Property->HasAnyPropertyFlags(CPF_Edit) && !IsManagedByEditor(*Property))
		{
			Property->CopyCompleteValue_InContainer(To, From);
		}
	}
}

bool FHapticEffectEditorProperties::AuthoredPropertiesIdentical(const UForceFeedbackEffectBase* A, const UForceFeedbackEffectBase* B, const UClass* EffectClass)
{
	for (TFieldIterator<FProperty> PropertyIt(EffectClass); PropertyIt; ++PropertyIt)
	{
		const FProperty* Property = *PropertyIt;
		if (Property->HasAnyPropertyFlags(CPF_Edit) && !IsManagedByEditor(*Property) && !Property->Identical_InContainer(A, B))
		{
			return false;
		}
	}

	return true;
}

bool FHapticEffectEditorProperties::FindEffectKind(const UClass* EffectClass, EHapticPreviewEffectKind& OutKind)
{
	if (!EffectClass)
	{
		return false;
	}

	if (EffectClass->IsChildOf(UForceFeedbackEffectPeriodic::StaticClass()))
	{
		OutKind = EHapticPreviewEffectKind::Periodic;
		return true;
	}

	if (EffectClass->IsChildOf(UForceFeedbackEffectConstant::StaticClass()))
	{
		OutKind = EHapticPreviewEffectKind::Constant;
		return true;
	}

	if (EffectClass->IsChildOf(UForceFeedbackEffectRamp::StaticClass()))
	{
		OutKind = EHapticPreviewEffectKind::Ramp;
		return true;
	}

	if (EffectClass->IsChildOf(UForceFeedbackEffectLeftRight::StaticClass()))
	{
		OutKind = EHapticPreviewEffectKind::LeftRight;
		return true;
	}

	return false;
}

UClass* FHapticEffectEditorProperties::GetEffectKindClass(const EHapticPreviewEffectKind Kind)
{
	switch (Kind)
	{
		case EHapticPreviewEffectKind::Periodic:
			return UForceFeedbackEffectPeriodic::StaticClass();
		case EHapticPreviewEffectKind::Constant:
			return UForceFeedbackEffectConstant::StaticClass();
		case EHapticPreviewEffectKind::Ramp:
			return UForceFeedbackEffectRamp::StaticClass();
		case EHapticPreviewEffectKind::LeftRight:
			return UForceFeedbackEffectLeftRight::StaticClass();
	}

	return nullptr;
}

bool FHapticEffectEditorProperties::IsSupportedEffectBlueprint(const FAssetData& AssetData)
{
	FString NativeParentClassPath;
	if (!AssetData.GetTagValue(FBlueprintTags::NativeParentClassPath, NativeParentClassPath))
	{
		return false;
	}

	EHapticPreviewEffectKind Kind;
	return FindEffectKind(FindObject<UClass>(nullptr, *FPackageName::ExportTextPathToObjectPath(NativeParentClassPath)), Kind);
}
