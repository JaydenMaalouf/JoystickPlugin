#pragma once

#include "IInputDevice.h"
#include "InputCoreTypes.h"
#include "Containers/Array.h"

#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickDeviceData.h"
#include "Data/JoystickInfo.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"

class FJoystickInputDevice final : public IInputDevice
{
public:
	explicit FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int ControllerId, const FForceFeedbackValues& Values) override;

	void JoystickPluggedIn(const FDeviceInfoSDL& Device);
	void JoystickUnplugged(int DeviceId);
	void JoystickButton(int DeviceId, int Button, bool Pressed);
	void JoystickAxis(int DeviceId, int Axis, float Value);
	void JoystickHat(int DeviceId, int Hat, EJoystickPOVDirection Value);
	void JoystickBall(int DeviceId, int Ball, FVector2D Value);

	bool GetDeviceData(int DeviceId, FJoystickDeviceData& DeviceData);
	bool GetDeviceInfo(int DeviceId, FJoystickInfo& DeviceInfo);
	bool GetKeyDeviceInfo(const FKey& Key, FJoystickInfo& DeviceInfo);
	int GetDeviceCount() const;
	int GetDeviceIdByKey(const FKey& Key);
	void GetDeviceIds(TArray<int>& DeviceIds) const;
	int GetDeviceIndexByKey(const FKey& Key);

	void SetPlayerOwnership(int DeviceId, int PlayerId);

	void ResetAxisProperties();
	void UpdateAxisProperties();

private:
	void InitialiseInputDevice(const FDeviceInfoSDL& Device);
	void InitialiseAxis(const int DeviceId, const FJoystickDeviceData& JoystickState, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseButtons(const int DeviceId, const FJoystickDeviceData& JoystickState, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseHats(const int DeviceId, const FJoystickDeviceData& JoystickState, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseBalls(const int DeviceId, const FJoystickDeviceData& JoystickState, const FString& BaseKeyName, const FString& BaseDisplayName);

	TMap<int, FJoystickDeviceData> JoystickDeviceData;
	TMap<int, FJoystickInfo> JoystickDeviceInfo;

	TMap<int, TArray<FKey>> DeviceButtonKeys;
	TMap<int, TArray<FKey>> DeviceAxisKeys;
	TMap<int, TArray<FKey>> DeviceHatKeys[2];
	TMap<int, TArray<FKey>> DeviceBallKeys[2];
	TMap<int, TArray<FKey>> DeviceKeys;

	const TArray<FString> AxisNames = {TEXT("X"), TEXT("Y")};

	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
};
