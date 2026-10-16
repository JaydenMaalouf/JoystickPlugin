// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "Data/JoystickHapticInformation.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"
#include "ForceFeedback/Debug/ForceFeedbackDebugOverlay.h"
#include "GameFramework/Actor.h"
#include "JoystickFunctionLibrary.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickLogManager.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

UForceFeedbackEffectBase::UForceFeedbackEffectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(-1)
	  , EffectId(-1)
	  , IsInitialised(false)
	  , Iterations(1)
	  , InfiniteIterations(false)
	  , Tickable(true)
	  , TickableInEditor(true)
	  , TickableWhenPaused(false)
	  , ForceStopAfterDurationLapsed(false)
	  , Effect()
	  , StartTime(UINT64_MAX)
	  , TimeAccumulator(0)
	  , EffectRunning(false)
	  , ShouldStartWithComponent(true)
#if !UE_BUILD_SHIPPING
	  , DebugWriteCount(0)
	  , DebugWriteTime(0.0f)
	  , DebugWritesPerSecond(0.0f)
#endif
{
	SDL_zero(Effect);
}

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

#if WITH_EDITOR
void UForceFeedbackEffectBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// The limit follows the duration, so editing it can leave Iterations above what can play.
	Iterations = FMath::Clamp<int64>(Iterations, 1, GetMaxIterations());
}
#endif

void UForceFeedbackEffectBase::Tick(const float DeltaTime)
{
	if (Configuration.UseEffectRate)
	{
		const float FixedTimeStep = 1.0f / Configuration.EffectHz;

		TimeAccumulator += DeltaTime;

		int SubTickCount = 0;
		while (TimeAccumulator >= FixedTimeStep && SubTickCount < Configuration.MaxSubticks)
		{
			TickEffect(FixedTimeStep);

			TimeAccumulator -= FixedTimeStep;
			SubTickCount++;
		}

		// Clamp accumulator to prevent spiral of death
		if (TimeAccumulator > FixedTimeStep * Configuration.MaxSubticks)
		{
			TimeAccumulator = FixedTimeStep;
		}
	}
	else
	{
		TickEffect(DeltaTime);
	}
}

bool UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect is already initialised"));
		return true;
	}

	if (InstanceId == -1)
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect InstanceId is not set"));
		return false;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: HapticDeviceManager is invalid"));
		return false;
	}

	CreateEffect();
	EffectId = HapticDeviceManager->CreateEffect(InstanceId, Effect);
	if (EffectId == -1)
	{
		return false;
	}

	IsInitialised = true;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return true;
	}

	OnInitialisedEffect();
	if (OnInitialisedEffectDelegate.IsBound())
	{
		OnInitialisedEffectDelegate.Broadcast(this);
	}

	if (Configuration.AutoStartOnInitialisation)
	{
		StartEffect();
	}

	return true;
}

bool UForceFeedbackEffectBase::StartEffect()
{
	if (IsInitialised == false)
	{
		return false;
	}

	const bool Status = GetEffectStatus();
	if (Status == true)
	{
		EffectRunning = true;
		return true;
	}

	if (!IsEffectValid())
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::StartEffect: Effect is invalid and won't be started"));
		return false;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return false;
	}

	if (Configuration.AutoUpdateOnStart)
	{
		// Starting only replays what the device last received, so send anything that has changed since
		UpdateEffect();
	}

	const uint32 RunIterations = GetRunIterations();
	if (InfiniteIterations == false && RunIterations < Iterations)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("UForceFeedbackEffectBase::StartEffect: %lld iterations exceeds the longest playable time, clamping to %u iterations"), Iterations, RunIterations);
	}

	const bool Result = HapticDeviceManager->RunEffect(InstanceId, EffectId, RunIterations);
	if (Result == false)
	{
		return false;
	}
	StartTime = UJoystickFunctionLibrary::GetTicks();
	EffectRunning = true;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return false;
	}

	OnStartedEffect();
	if (OnStartedEffectDelegate.IsBound())
	{
		OnStartedEffectDelegate.Broadcast(this);
	}

	return true;
}

void UForceFeedbackEffectBase::StopEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	const bool Result = HapticDeviceManager->StopEffect(InstanceId, EffectId);
	if (Result == false)
	{
		return;
	}
	StartTime = UINT64_MAX;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

	OnStoppedEffect();
	if (OnStoppedEffectDelegate.IsBound())
	{
		OnStoppedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::DestroyEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	HapticDeviceManager->DestroyEffect(InstanceId, EffectId);

	IsInitialised = false;
	EffectId = -1;
	StartTime = UINT64_MAX;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

	OnDestroyedEffect();
	if (OnDestroyedEffectDelegate.IsBound())
	{
		OnDestroyedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::UpdateEffect()
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	UpdateEffectData();
	const bool Result = HapticDeviceManager->UpdateEffect(InstanceId, EffectId, Effect);
	if (Result == false)
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	DebugWriteCount++;
#endif

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

	OnUpdatedEffect();
	if (OnUpdatedEffectDelegate.IsBound())
	{
		OnUpdatedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::OnInitialisedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnStartedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnStoppedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnUpdatedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnDestroyedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::TickEffect(const float DeltaTime)
{
	if (!IsInitialised || this->IsUnreachable())
	{
		return;
	}

	ReceivedTick(DeltaTime);

	if (Configuration.AutoUpdatePostTick)
	{
		UpdateEffect();
	}

#if !UE_BUILD_SHIPPING
	UpdateDebugWriteRate(DeltaTime);
#endif

	// Effect should be finished but sometimes SDL doesn't handle the status correctly.
	if (ForceStopAfterDurationLapsed)
	{
		const uint32 Duration = GetEffectDuration();
		const uint32 RunIterations = GetRunIterations();

		const bool ShouldCheck =
			EffectRunning &&
			InfiniteIterations == false &&
			RunIterations != SDL_HAPTIC_INFINITY &&
			StartTime != UINT64_MAX &&
			Duration != SDL_HAPTIC_INFINITY &&
			Duration != 0;

		if (ShouldCheck)
		{
			const uint64 CurrentTime = UJoystickFunctionLibrary::GetTicks();

			// The effect waits out its delay, then runs once per iteration, so it only lapses after the last one.
			// Both operands are 32-bit, so the duration is widened first to keep a long total from wrapping.
			if (CurrentTime - StartTime >= GetEffectDelay() + static_cast<uint64>(Duration) * RunIterations && GetEffectStatus())
			{
				StopEffect();
			}
		}
	}
}

void UForceFeedbackEffectBase::ReceivedTick_Implementation(const float DeltaTime)
{
}

bool UForceFeedbackEffectBase::GetEffectStatus() const
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetMutableDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return false;
	}

	return HapticDeviceManager->GetEffectStatus(InstanceId, EffectId);
}

bool UForceFeedbackEffectBase::IsEffectRunning() const
{
	return EffectRunning;
}

float UForceFeedbackEffectBase::GetElapsedTime() const
{
	return GetElapsedTimeMilliseconds() / 1000.0f;
}

uint64 UForceFeedbackEffectBase::GetElapsedTimeMilliseconds() const
{
	if (!EffectRunning || StartTime == UINT64_MAX)
	{
		return 0;
	}

	return UJoystickFunctionLibrary::GetTicks() - StartTime;
}

void UForceFeedbackEffectBase::SetInstanceId(const FJoystickInstanceId& NewInstanceId)
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Cannot update Effect InstanceId post initialisation"));
		return;
	}

	InstanceId = NewInstanceId;
}

const FJoystickInstanceId& UForceFeedbackEffectBase::GetInstanceId() const
{
	return InstanceId;
}

void UForceFeedbackEffectBase::SetTickable(const bool NewTickable)
{
	Tickable = NewTickable;
}

void UForceFeedbackEffectBase::SetTickableInEditor(const bool NewTickableInEditor)
{
	TickableInEditor = NewTickableInEditor;
}

void UForceFeedbackEffectBase::SetTickableWhenPaused(const bool NewTickableWhenPaused)
{
	TickableWhenPaused = NewTickableWhenPaused;
}

AActor* UForceFeedbackEffectBase::GetOwningActor() const
{
	return GetOwningActor<AActor>();
}

bool UForceFeedbackEffectBase::IsSupportedByDevice(const FJoystickHapticInformation& HapticInformation)
{
	if (!IsInitialised)
	{
		CreateEffect();
	}

	switch (Effect.type)
	{
		case SDL_HAPTIC_CONSTANT:
			return HapticInformation.Constant;
		case SDL_HAPTIC_SINE:
			return HapticInformation.Sine;
		case SDL_HAPTIC_TRIANGLE:
			return HapticInformation.Triangle;
		case SDL_HAPTIC_SAWTOOTHUP:
			return HapticInformation.SawToothUp;
		case SDL_HAPTIC_SAWTOOTHDOWN:
			return HapticInformation.SawToothDown;
		case SDL_HAPTIC_RAMP:
			return HapticInformation.Ramp;
		case SDL_HAPTIC_SPRING:
			return HapticInformation.Spring;
		case SDL_HAPTIC_DAMPER:
			return HapticInformation.Damper;
		case SDL_HAPTIC_INERTIA:
			return HapticInformation.Inertia;
		case SDL_HAPTIC_FRICTION:
			return HapticInformation.Friction;
		case SDL_HAPTIC_LEFTRIGHT:
			return HapticInformation.LeftRight;
		case SDL_HAPTIC_CUSTOM:
			return HapticInformation.Custom;
		default:
			// Not a type we track support for, let SDL decide when the effect is created.
			return true;
	}
}

bool UForceFeedbackEffectBase::IsEffectValid()
{
	return GetEffectDuration() != 0;
}

void UForceFeedbackEffectBase::DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const
{
	OutInfo.Effect = FObjectKey(this);
	OutInfo.Label = EffectName.IsNone() ? GetClass()->GetName() : EffectName.ToString();
	if (GroupTag.IsValid())
	{
		OutInfo.Label = FString::Printf(TEXT("%s / %s"), *GroupTag.ToString(), *OutInfo.Label);
	}
	OutInfo.InstanceId = InstanceId;
	OutInfo.IsRunning = EffectRunning;

#if !UE_BUILD_SHIPPING
	OutInfo.WritesPerSecond = DebugWritesPerSecond;
	OutInfo.Values = DebugValues;
	OutInfo.TextValues = DebugTextValues;
#endif
}

void UForceFeedbackEffectBase::AddDebugValue(const FName Name, const float Value)
{
#if !UE_BUILD_SHIPPING
	if (FForceFeedbackDebugOverlay::IsEnabled())
	{
		SetDebugValue(DebugValues, Name, Value);
	}
#endif
}

void UForceFeedbackEffectBase::AddDebugText(const FName Name, const FName Value)
{
#if !UE_BUILD_SHIPPING
	if (FForceFeedbackDebugOverlay::IsEnabled())
	{
		SetDebugValue(DebugTextValues, Name, Value);
	}
#endif
}

#if !UE_BUILD_SHIPPING
template <typename ListType, typename ValueType>
void UForceFeedbackEffectBase::SetDebugValue(ListType& List, const FName Name, const ValueType& Value)
{
	for (TPair<FName, ValueType>& Entry : List)
	{
		if (Entry.Key == Name)
		{
			Entry.Value = Value;
			return;
		}
	}

	List.Emplace(Name, Value);
}

void UForceFeedbackEffectBase::UpdateDebugWriteRate(const float DeltaTime)
{
	DebugWriteTime += DeltaTime;
	if (DebugWriteTime < DebugWriteRateWindow)
	{
		return;
	}

	DebugWritesPerSecond = DebugWriteCount / DebugWriteTime;
	DebugWriteCount = 0;
	DebugWriteTime = 0.0f;
}
#endif

uint32 UForceFeedbackEffectBase::GetEffectDuration() const
{
	return -1;
}

uint32 UForceFeedbackEffectBase::GetEffectDelay() const
{
	return 0;
}

uint32 UForceFeedbackEffectBase::GetRunIterations() const
{
	if (InfiniteIterations)
	{
		return SDL_HAPTIC_INFINITY;
	}

	const uint32 RunIterations = static_cast<uint32>(FMath::Clamp<int64>(Iterations, 0, SDL_HAPTIC_INFINITY));
	if (RunIterations == SDL_HAPTIC_INFINITY)
	{
		return RunIterations;
	}

	return FMath::Min(RunIterations, GetPlatformMaxIterations());
}

uint32 UForceFeedbackEffectBase::GetPlatformMaxIterations() const
{
	return SDL_HAPTIC_INFINITY;
}

uint32 UForceFeedbackEffectBase::GetMaxIterations() const
{
	return GetEffectDuration() == SDL_HAPTIC_INFINITY ? 1 : GetPlatformMaxIterations();
}

void UForceFeedbackEffectBase::CreateEffect()
{
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));
	UpdateEffectData();
}

void UForceFeedbackEffectBase::UpdateEffectData()
{
}

float UForceFeedbackEffectBase::GetEvaluationTime() const
{
	const int64 ElapsedTime = static_cast<int64>(GetElapsedTimeMilliseconds());
	const int64 Delay = GetEffectDelay();
	if (!EffectRunning || ElapsedTime < Delay)
	{
		return -1.0f;
	}

	const uint32 Duration = GetEffectDuration();
	if (Duration == SDL_HAPTIC_INFINITY || Duration == 0)
	{
		return (ElapsedTime - Delay) / 1000.0f;
	}

	int64 Iteration;
	int64 PassTime;
	if (!FForceFeedbackEffectEvaluation::GetIterationProgress(ElapsedTime, Delay, Duration, GetRunIterations(), Iteration, PassTime))
	{
		return -1.0f;
	}

	return PassTime / 1000.0f;
}
