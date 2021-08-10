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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ToolTip = "A vector describing the direction and magnitude of the effect on each axis. Each individual axis has a range of -1.0 to 1.0 and is independent of the other axes. Specifying a negative value for an axis reverses the input values from the axis."))
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Duration", ToolTip = "The duration of the effect."))
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Bias", ToolTip = "The offset to the center point in effect calculations. Range is from -1.0 to 1.0."))
		FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Dead Zone", ToolTip = "Specifies the value below which the force feedback is not applied. Range is from 0.0 to 1.0 and is applied asymmetrically around the center point."))
		FVector Deadband = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ShortToolTip = "Positive Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the positive direction along the specified axis. Range is from -infinity to +infinity."))
		FVector LeftCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", ShortToolTip = "Max Positive Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the positive direction along the specified axis. Range is from 0 to 1.0."))
		FVector LeftSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Negative Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the negative direction along the specified axis. Range is from -infinity to +infinity."))
		FVector RightCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", ShortToolTip = "Max Negative Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the negative direction along the specified axis. Range is from 0 to 1.0."))
		FVector RightSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float RampStart = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		float RampEnd = 1.0f;
};

