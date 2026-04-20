#pragma once
#include "Instance/Instance.hpp"
#include "Mouse/Mouse.hpp"
#include "Character/Character.hpp"

class PlayerClass : public Instance {
public:
	PlayerClass() = default;
	PlayerClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	PlayerClass(const Instance & instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	int UserId();
	std::string DisplayName();
	MouseClass Mouse();
	CharacterClass Character();
	int CharacterAppearanceId();
};