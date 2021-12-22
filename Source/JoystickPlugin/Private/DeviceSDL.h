#pragma once

#include "JoystickDeviceManager.h"

#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickDeviceData.h"
#include "Interfaces/JoystickEventInterface.h"

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
	explicit FDeviceSDL(IJoystickEventInterface* JoystickEventInterface);
	virtual ~FDeviceSDL();
	
	FJoystickDeviceData InitialDeviceState(int32 DeviceId);

	static FString DeviceGUIDtoString(int32 DeviceIndex);
	static FGuid DeviceIndexToGUID(int32 DeviceIndex);

	FDeviceInfoSDL * GetDevice(int32 DeviceId);
	
	void IgnoreGameControllers(bool bIgnore);
	//void SetJoystickRumble(FDeviceId DeviceId, FForceFeedbackValues values);

	void Update();

	void Init();

private:
	FDeviceInfoSDL AddDevice(int32 DeviceIndex);
	void RemoveDevice(int32 DeviceId);

	TMap<int32, FDeviceInfoSDL> Devices;
	TMap<int32, int32> DeviceMapping;
	
	IJoystickEventInterface* EventInterface;
	
	bool bIgnoreGameControllers = true;
	bool bOwnsSDL;

	static int HandleSDLEvent(void* Userdata, SDL_Event* Event);
};
