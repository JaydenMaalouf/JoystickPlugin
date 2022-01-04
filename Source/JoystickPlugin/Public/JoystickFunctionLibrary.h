#pragma once

#include "Data/JoystickPOVDirection.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class UJoystickFunctionLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FVector2D POVAxis(EJoystickPOVDirection Direction);
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions", meta = (DeterminesOutputType = "ClassType"))
		static UForceFeedbackEffectBase* CreateEffect(UObject* Outer, TSubclassOf<class UForceFeedbackEffectBase> ClassType, bool AutoInitialise, const bool AutoStart);
	
	static EJoystickPOVDirection HatValueToDirection(int8 Value);
};
