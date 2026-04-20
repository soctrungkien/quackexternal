#pragma once
#include "Instance/Instance.hpp"

class MouseClass : public Instance {
public:
	MouseClass() = default;
	MouseClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	MouseClass(const Instance & instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	Vector2 Position();
};