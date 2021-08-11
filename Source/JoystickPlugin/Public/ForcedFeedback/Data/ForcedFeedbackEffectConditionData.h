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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = "-32768", ClampMax = "32767", ShortToolTip = "Positive Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the positive direction along the specified axis. Range is from -infinity to +infinity."))
		FIntVector LeftCoeff = FIntVector::ZeroValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535", ShortToolTip = "Max Positive Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the positive direction along the specified axis. Range is from 0 to 1.0."))
		FIntVector LeftSat = FIntVector::ZeroValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767", ShortToolTip = "Negative Coefficient", ToolTip = "The slope of the line describing how rapidly the force increases as the input moves away from the center point in the negative direction along the specified axis. Range is from -infinity to +infinity."))
		FIntVector RightCoeff = FIntVector::ZeroValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535", ShortToolTip = "Max Negative Magnitude", ToolTip = "The maximum magnitude of the force feedback as the input moves away from the center point in the negative direction along the specified axis. Range is from 0 to 1.0."))
		FIntVector RightSat = FIntVector::ZeroValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535", ShortToolTip = "Dead Zone", ToolTip = "Specifies the value below which the force feedback is not applied. Range is from 0.0 to 1.0 and is applied asymmetrically around the center point."))
		FIntVector Deadband = FIntVector::ZeroValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767", ShortToolTip = "Bias", ToolTip = "The offset to the center point in effect calculations. Range is from -1.0 to 1.0."))
		FIntVector Center = FIntVector::ZeroValue;
};

