#pragma once

#include "Interfaces/JoystickPluginInterface.h"
#include "JoystickInputDevice.h"

class FJoystickPlugin : public IJoystickPlugin
{

public:
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	virtual void ShutdownModule() override;
	virtual void StartupModule() override;
	
	TSharedPtr<FJoystickInputDevice> GetJoystickInputDevice() const { return JoystickInputDevice; }

private:

	void* SdlDllHandle = nullptr;
	
	TSharedPtr<FJoystickInputDevice> JoystickInputDevice;
};
