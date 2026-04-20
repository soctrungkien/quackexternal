#pragma once
#include "Instance/Instance.hpp"

class ModelClass : public Instance {
public:
	ModelClass() = default;
	ModelClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	// Functions
};