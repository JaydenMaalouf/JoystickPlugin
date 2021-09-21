#include "ForceFeedback/JoystickMultiForceFeedbackComponent.h"

void UJoystickMultiForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectTypes.Num() == 0) 
	{
		return;
	}

	for (auto& effectType : EffectTypes)
	{
		UForceFeedbackEffectBase* effect = NewObject<UForceFeedbackEffectBase>(this, effectType.Key);
		if (effect == nullptr || effect->IsPendingKill())
		{
			continue;
		}

		effect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnInitialisedEffect);
		effect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStartedEffect);
		effect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStoppedEffect);
		effect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnUpdatedEffect);
		effect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnDestroyedEffect);

		effect->AutoStartOnInit = effectType.Value.AutoStartOnInit;

		if (effectType.Value.AutoInit)
		{
			effect->InitialiseEffect();
		}

		Effects.Add(effect);
	}
}

void UJoystickMultiForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effects.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < Effects.Num(); i++)
	{
		UForceFeedbackEffectBase* effect = Effects[i];
		if (effect == nullptr || effect->IsPendingKill())
		{
			continue;
		}

		effect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnInitialisedEffect);
		effect->OnStartedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStartedEffect);
		effect->OnStoppedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStoppedEffect);
		effect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnUpdatedEffect);
		effect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnDestroyedEffect);
	}
	
	Effects.Empty();
}

void UJoystickMultiForceFeedbackComponent::OnInitialisedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickMultiForceFeedbackComponent::OnStartedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickMultiForceFeedbackComponent::OnStoppedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickMultiForceFeedbackComponent::OnUpdatedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickMultiForceFeedbackComponent::OnDestroyedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

TArray<UForceFeedbackEffectBase*> UJoystickMultiForceFeedbackComponent::GetEffects()
{
	return Effects;
}

UForceFeedbackEffectBase* UJoystickMultiForceFeedbackComponent::GetEffectByType(TSubclassOf<class UForceFeedbackEffectBase> effectType)
{
	for (int32 i = 0; i < Effects.Num(); i++)
	{
		UForceFeedbackEffectBase* effect = Effects[i];
		if (effect->GetClass() == effectType)
		{
			return effect;
		}
	}

	return nullptr;
}

void UJoystickMultiForceFeedbackComponent::StartEffect(UForceFeedbackEffectBase* effect)
{
	if (effect == nullptr)
	{
		return;
	}

	effect->StartEffect();
}

void UJoystickMultiForceFeedbackComponent::StopEffect(UForceFeedbackEffectBase* effect)
{
	if (effect == nullptr)
	{
		return;
	}

	effect->StopEffect();
}
