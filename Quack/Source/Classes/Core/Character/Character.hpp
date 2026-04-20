#pragma once
#include "Model/Model.hpp"

class CharacterClass : public ModelClass {
public:
	CharacterClass() = default;
	CharacterClass(HANDLE handle, uintptr_t address) :
		ModelClass(handle, address)
	{}

	CharacterClass(const ModelClass& instance) :
		ModelClass(instance.Handle, instance.Self)
	{}

	// Functions
	std::string RigType();
};