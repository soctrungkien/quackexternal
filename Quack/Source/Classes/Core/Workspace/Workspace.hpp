#pragma once
#include "Instance/Instance.hpp"

class Workspace : public Instance {
public:
	Workspace() = default;
	Workspace(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	Workspace(const Instance & instance) :
		Instance(instance.Handle, instance.Self)
	{}
};