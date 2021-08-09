#pragma once

struct FInstanceId
{
	int32 value = -1;

	explicit FInstanceId(int32 v) : value(v) {}
	
	bool operator==(FInstanceId other) const 
	{ 
		return value == other.value; 
	};
};

FORCEINLINE uint32 GetTypeHash(FInstanceId instanceId)
{
	return GetTypeHash(instanceId.value);
}