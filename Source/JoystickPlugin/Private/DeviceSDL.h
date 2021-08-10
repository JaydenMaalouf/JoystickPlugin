#pragma once

#include "Interfaces/JoystickInterface.h"
#include "JoystickDeviceManager.h"

#include "Data/DeviceIndex.h"
#include "Data/DeviceId.h"
#include "Data/InstanceId.h"
#include "Data/DeviceInfoSDL.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

THIRD_PARTY_INCLUDES_END

DECLARE_LOG_CATEGORY_EXTERN(JoystickPluginLog, Log, All);

union SDL_Event;

class FDeviceSDL
{
public:
	FJoystickState InitialDeviceState(FDeviceId DeviceId);

	static FString DeviceGUIDtoString(FDeviceIndex DeviceIndex);
	static FGuid DeviceGUIDtoGUID(FDeviceIndex DeviceIndex);

	FDeviceInfoSDL * GetDevice(FDeviceId DeviceId);
	
	void IgnoreGameControllers(bool bIgnore);

	void Update();

	FDeviceSDL(IJoystickEventInterface* EventInterface);
	void Init();
	virtual ~FDeviceSDL();

private:
	FDeviceInfoSDL AddDevice(FDeviceIndex DeviceIndex);
	void RemoveDevice(FDeviceId DeviceId);

	TMap<FDeviceId, FDeviceInfoSDL> Devices;	
	TMap<FInstanceId, FDeviceId> DeviceMapping;

	IJoystickEventInterface* EventInterface;

	bool bOwnsSDL;

	bool bIgnoreGameControllers = true;

	bool bSubSystemHaptic;
	bool bSubSystemJoystick;
	bool bSubSystemGameController;

	static int HandleSDLEvent(void* Userdata, SDL_Event* Event);
};
