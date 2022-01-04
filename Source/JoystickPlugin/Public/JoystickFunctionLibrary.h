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
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions", meta = (DeterminesOutputType = "ClassType", HidePin = "Outer", DefaultToSelf = "Outer"))
		static UForceFeedbackEffectBase* CreateEffect(UObject* Outer, TSubclassOf<class UForceFeedbackEffectBase> ClassType, bool AutoInitialise = true, const bool AutoStart = false);
	
	static EJoystickPOVDirection HatValueToDirection(int8 Value);
};
