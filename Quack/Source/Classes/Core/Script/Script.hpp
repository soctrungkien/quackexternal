#pragma once
#include "Instance/Instance.hpp"

class ScriptClass : public Instance {
public:
	ScriptClass() = default;
	ScriptClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	ScriptClass(const Instance& instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	std::string GetBytecode();
	void SetBytecode(const std::string& bytecode, bool revert);
	std::string GetHash();
	int RunContext();
	void RunContext(Offsets::Enums::RunContext context);
};