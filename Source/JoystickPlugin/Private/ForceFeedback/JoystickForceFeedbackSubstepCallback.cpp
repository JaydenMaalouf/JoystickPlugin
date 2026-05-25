// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackSubstepCallback.h"

#include "ForceFeedback/JoystickForceFeedbackComponent.h"

void FJoystickForceFeedbackSubstepCallback::Init(UJoystickForceFeedbackComponent* InComponent)
{
	Component = InComponent;
}

void FJoystickForceFeedbackSubstepCallback::OnPreSimulate_Internal()
{
	if (UJoystickForceFeedbackComponent* JoystickComponent = Component.Get())
	{
		JoystickComponent->TickEffects(static_cast<float>(GetDeltaTime_Internal()));
	}
}

FName FJoystickForceFeedbackSubstepCallback::GetFNameForStatId() const
{
	static constexpr FLazyName StaticName(TEXT("FJoystickForceFeedbackSubstepCallback"));
	return StaticName;
}
