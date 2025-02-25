// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "IInputDevice.h"
#include "Data/JoystickDeviceState.h"

enum class EHatDirection : uint8;
struct FJoystickInstanceId;
struct FDeviceInfoSDL;

class JOYSTICKPLUGIN_API FJoystickInputDevice final : public IInputDevice
{
public:
	explicit FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int ControllerId, const FForceFeedbackValues& Values) override;
	virtual bool IsGamepadAttached() const override;

	void JoystickPluggedIn(const FDeviceInfoSDL& Device);
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId) const;
	void JoystickButton(const FJoystickInstanceId& InstanceId, const int Button, const bool Pressed);
	void JoystickAxis(const FJoystickInstanceId& InstanceId, const int Axis, const float Value);
	void JoystickHat(const FJoystickInstanceId& InstanceId, const int Hat, const EHatDirection Value);
	void JoystickBall(const FJoystickInstanceId& InstanceId, const int Ball, const FVector2D& Value);
	void JoystickGyro(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value);
	void JoystickAccelerometer(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value);

	FJoystickDeviceState* GetDeviceData(const FJoystickInstanceId& InstanceId);
	FJoystickInstanceId GetInstanceIdByKey(const FKey& Key) const;
	int GetAxisIndexFromKey(const FKey& Key) const;

	void ResetAxisProperties();
	void UpdateAxisProperties();

private:
	void InitialiseAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseHatAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseHatButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseBalls(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);

	void TrySetCustomDisplayName(FString& ButtonDisplayName, const FKey& ButtonKey);
	void TryAddWidgetNavigation(const FKey& ButtonKey);

	TMap<FJoystickInstanceId, FJoystickDeviceState> JoystickDeviceState;

	TMap<FJoystickInstanceId, TArray<FKey>> DeviceButtonKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceAxisKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceHatAxisKeys[2];
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceHatButtonKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceBallKeys[2];
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceKeys;

	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
};
