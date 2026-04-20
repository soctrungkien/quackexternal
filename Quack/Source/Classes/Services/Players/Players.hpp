#pragma once
#include "Instance/Instance.hpp"
#include "DataModel/DataModel.hpp"
#include "Player/Player.hpp"

class PlayersClass : public Instance {
public:
	PlayersClass() = default;
	PlayersClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	PlayersClass(const Instance& instance) :
		Instance(instance.Handle, instance.Self)
	{}

	// Functions
	PlayerClass LocalPlayer(const int timeout = 100000);
};