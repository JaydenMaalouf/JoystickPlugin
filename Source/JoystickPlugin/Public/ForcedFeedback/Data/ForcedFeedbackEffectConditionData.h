#pragma once

#include "ForcedFeedbackEffectBaseData.h"
#include "ForcedFeedback/Types/ForcedFeedbackConditionEffectType.h"
#include "ForcedFeedbackEffectConditionData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectConditionData : public FForcedFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForcedFeedbackConditionEffectType EffectType = EForcedFeedbackConditionEffectType::SPRING;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ShortToolTip = "Positive Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the positive direction along the specified axis. Range is from -infinity to +infinity."))
		FVector LeftCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Max Positive Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the positive direction along the specified axis. Range is from 0 to 1.0."))
		FVector LeftSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Negative Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the negative direction along the specified axis. Range is from -infinity to +infinity."))
		FVector RightCoeff = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Max Negative Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the negative direction along the specified axis. Range is from 0 to 1.0."))
		FVector RightSat = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Dead Zone", ToolTip = "Specifies the value below which the force feedback is not applied. Range is from 0.0 to 1.0 and is applied asymmetrically around the center point."))
		FVector Deadband = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Bias", ToolTip = "The offset to the center point in effect calculations. Range is from -1.0 to 1.0."))
		FVector Center = FVector::ZeroVector;
};

