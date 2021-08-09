#pragma once

struct FDeviceIndex
{
	int32 value = -1;
    
	explicit FDeviceIndex(int32 v) : value(v) {}
    
	bool operator==(const FDeviceIndex other) const 
    { 
        return value == other.value; 
    };
};