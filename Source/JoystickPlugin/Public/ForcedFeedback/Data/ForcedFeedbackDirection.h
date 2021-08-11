#pragma once

#include "ForcedFeedback/Types/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackDirection.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackDirection
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForcedFeedbackDirectionType DirectionType = EForcedFeedbackDirectionType::CARTESIAN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-1", ClampMax = "1", ToolTip = "A vector describing the direction and magnitude of the effect on each axis. Each individual axis has a range of -1.0 to 1.0 and is independent of the other axes. Specifying a negative value for an axis reverses the input values from the axis."))
		FIntVector Direction = FIntVector::ZeroValue;
};

