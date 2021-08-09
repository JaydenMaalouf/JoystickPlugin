#pragma once

#include "ForcedFeedbackEffectType.h"
#include "ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForcedFeedbackDirectionType DirectionType = EForcedFeedbackDirectionType::CARTESIAN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Direction = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float Level = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float Period = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float Magnitude=1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Offset=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float Phase=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Delay=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float Length=1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		bool InfiniteLength = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float AttackLength=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float AttackLevel=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLength=0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLevel=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Deadband = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		FVector LeftCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector LeftSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector RightCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		FVector RightSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float RampStart = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float RampEnd = 1.0f;
};

