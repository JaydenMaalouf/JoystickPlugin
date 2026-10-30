// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"

#include "JoystickInputDeviceForceFeedbackProperties.generated.h"

// Corrections for a device whose driver reads a condition effect's sides the opposite way to the DirectInput/SDL spec, where the right/positive values apply when the input is on the positive side.
// Inverting sends the Left values as Right (and the reverse) and mirrors the centre, so the authored effect behaves as written on that device.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceForceFeedbackProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceForceFeedbackProperties()
		: InvertConditions(false)
		  , InvertSpring(false)
		  , InvertDamper(false)
		  , InvertInertia(false)
		  , InvertFriction(false)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Force Feedback Properties", meta=(ToolTip="The driver reads every condition type's sides the wrong way round. Inverts all of them, whatever the per-type settings below say."))
	bool InvertConditions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Force Feedback Properties", meta=(EditCondition="!InvertConditions", ToolTip="The driver applies a spring's Left values when the wheel is to the right of centre, and the reverse."))
	bool InvertSpring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Force Feedback Properties", meta=(EditCondition="!InvertConditions", ToolTip="The driver applies a damper's Left values when the wheel turns right, and the reverse."))
	bool InvertDamper;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Force Feedback Properties", meta=(EditCondition="!InvertConditions", ToolTip="The driver applies inertia's Left values when the wheel accelerates right, and the reverse."))
	bool InvertInertia;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Force Feedback Properties", meta=(EditCondition="!InvertConditions", ToolTip="The driver applies friction's Left values when the wheel turns right, and the reverse."))
	bool InvertFriction;

	bool IsInverted(const EForceFeedbackConditionEffectType ConditionType) const
	{
		if (InvertConditions)
		{
			return true;
		}

		switch (ConditionType)
		{
			case EForceFeedbackConditionEffectType::Spring:
				return InvertSpring;
			case EForceFeedbackConditionEffectType::Damper:
				return InvertDamper;
			case EForceFeedbackConditionEffectType::Inertia:
				return InvertInertia;
			case EForceFeedbackConditionEffectType::Friction:
				return InvertFriction;
		}

		return false;
	}

	void UpdateProperties(const FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties)
	{
		InvertConditions = ForceFeedbackProperties.InvertConditions;
		InvertSpring = ForceFeedbackProperties.InvertSpring;
		InvertDamper = ForceFeedbackProperties.InvertDamper;
		InvertInertia = ForceFeedbackProperties.InvertInertia;
		InvertFriction = ForceFeedbackProperties.InvertFriction;
	}
};
