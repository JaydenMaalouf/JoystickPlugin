#pragma once

struct FDeviceId
{
	int32 value = -1;

	explicit FDeviceId(int32 v) : value(v) {}

	bool operator==(FDeviceId other) const 
    {
        return value == other.value; 
    };
};

FORCEINLINE uint32 GetTypeHash(FDeviceId deviceId)
{
	return GetTypeHash(deviceId.value);
}