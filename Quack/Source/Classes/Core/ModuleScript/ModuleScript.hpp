#pragma once
#include "Instance/Instance.hpp"

class ModuleScriptClass : public Instance {
public:
	ModuleScriptClass() = default;
	ModuleScriptClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	ModuleScriptClass(const Instance& instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	std::string GetBytecode();
	void SetBytecode(const std::string& bytecode, bool revert);
	std::string GetHash();
	uintptr_t IsCoreScript();
	void IsCoreScript(Offsets::Enums::ModuleType type);
	int RunContext();
	void RunContext(Offsets::Enums::RunContext context);
};