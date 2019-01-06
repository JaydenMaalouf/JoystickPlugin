#pragma once

#include "FeedbackEffect.Generated.h"

UENUM(BlueprintType)
enum class EFeedbackType : uint8 
{
	CONSTANT,
	SPRING,
	DAMPER,
	INERTIA,
	FRICTION,
	SINE,
	TRIANGLE,
	SAWTOOTHUP,
	SAWTOOTHDOWN,
	RAMP
	//LEFTRIGHT,
	//CUSTOM
};

USTRUCT(BlueprintType)
struct FFeedbackData{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EFeedbackType Type = EFeedbackType::SINE;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Direction=FVector(1,0,0);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Level = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Period = 0.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Magnitude=1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Offset=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Phase=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Delay=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Length=1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool InfiniteLength = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float AttackLength=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float AttackLevel=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float FadeLength=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float FadeLevel=0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Center = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Deadband = FVector(0, 0, 0);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector LeftCoeff = FVector(1, 1, 1);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector LeftSat = FVector(1, 1, 1);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector RightCoeff = FVector(1, 1, 1);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector RightSat = FVector(1, 1, 1);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float RampStart = -1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float RampEnd = 1.0f;
};