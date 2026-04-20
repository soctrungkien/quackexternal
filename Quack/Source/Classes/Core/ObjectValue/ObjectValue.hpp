#pragma once
#include "Instance/Instance.hpp"

class ObjectValueClass : public Instance {
public:
	ObjectValueClass() = default;
	ObjectValueClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	ObjectValueClass(const Instance& instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	Instance Value() const;
};