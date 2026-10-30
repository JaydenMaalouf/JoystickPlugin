// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/HapticEffectEditorProperties.h"

#include "AssetRegistry/AssetData.h"
#include "Data/JoystickHapticInformation.h"
#include "Engine/Blueprint.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectCondition.h"
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

	if (EffectClass->IsChildOf(UForceFeedbackEffectCondition::StaticClass()))
	{
		OutKind = EHapticPreviewEffectKind::Condition;
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
		case EHapticPreviewEffectKind::Condition:
			return UForceFeedbackEffectCondition::StaticClass();
		case EHapticPreviewEffectKind::LeftRight:
			return UForceFeedbackEffectLeftRight::StaticClass();
	}

	return nullptr;
}

bool FHapticEffectEditorProperties::IsEffectKindSupported(const EHapticPreviewEffectKind Kind, const FJoystickHapticInformation& HapticInformation)
{
	switch (Kind)
	{
		case EHapticPreviewEffectKind::Periodic:
			return HapticInformation.Sine || HapticInformation.Triangle || HapticInformation.SawToothUp || HapticInformation.SawToothDown || HapticInformation.Square;
		case EHapticPreviewEffectKind::Constant:
			return HapticInformation.Constant;
		case EHapticPreviewEffectKind::Ramp:
			return HapticInformation.Ramp;
		case EHapticPreviewEffectKind::Condition:
			return HapticInformation.Spring || HapticInformation.Damper || HapticInformation.Inertia || HapticInformation.Friction;
		case EHapticPreviewEffectKind::LeftRight:
			return HapticInformation.LeftRight;
	}

	return false;
}

bool FHapticEffectEditorProperties::IsWaveformSupported(const EForceFeedbackPeriodicEffectType WaveformType, const FJoystickHapticInformation& HapticInformation)
{
	switch (WaveformType)
	{
		case EForceFeedbackPeriodicEffectType::Sine:
			return HapticInformation.Sine;
		case EForceFeedbackPeriodicEffectType::Triangle:
			return HapticInformation.Triangle;
		case EForceFeedbackPeriodicEffectType::SawtoothUp:
			return HapticInformation.SawToothUp;
		case EForceFeedbackPeriodicEffectType::SawtoothDown:
			return HapticInformation.SawToothDown;
		case EForceFeedbackPeriodicEffectType::Square:
			return HapticInformation.Square;
	}

	return false;
}

bool FHapticEffectEditorProperties::IsConditionTypeSupported(const EForceFeedbackConditionEffectType ConditionType, const FJoystickHapticInformation& HapticInformation)
{
	switch (ConditionType)
	{
		case EForceFeedbackConditionEffectType::Spring:
			return HapticInformation.Spring;
		case EForceFeedbackConditionEffectType::Damper:
			return HapticInformation.Damper;
		case EForceFeedbackConditionEffectType::Inertia:
			return HapticInformation.Inertia;
		case EForceFeedbackConditionEffectType::Friction:
			return HapticInformation.Friction;
	}

	return false;
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
