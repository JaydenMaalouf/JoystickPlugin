#pragma once

struct FPropertyInformation
{
	FProperty* Property;
	void* PropertyValuePtr;
	FString DisplayName;
	FString PropertyPath;
	float MinValue;
	float MaxValue;
	TOptional<EUnit> Unit;
	int32 DiscoveryOrder = 0; // Order in which the property was discovered by the iterator
};
