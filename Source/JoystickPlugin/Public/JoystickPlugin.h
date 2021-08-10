#pragma once

#include "Interfaces/JoystickPluginInterface.h"
#include "JoystickDevice.h"

class FJoystickPlugin : public IJoystickPlugin
{

public:
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	void ShutdownModule() override;

	void StartupModule() override;

	TSharedPtr<class FJoystickDevice> JoystickDevice;
};
