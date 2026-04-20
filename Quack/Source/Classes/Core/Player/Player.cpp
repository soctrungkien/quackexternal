#include "Player.hpp"

int PlayerClass::UserId() {
	auto number = Memory::Read<int>(Handle, Self + Offsets::LocalPlayer::UserId);
	return number;
}

std::string PlayerClass::DisplayName() {
	auto ptr = Memory::Read<uintptr_t>(Handle, Self + Offsets::LocalPlayer::DisplayName);
	auto name = Memory::Read_String(Handle, ptr);
	if (name.empty()) {
		return this->Name();
	}
	return name;
}

MouseClass PlayerClass::Mouse() {
	auto mousePtr = Memory::Read<uintptr_t>(Handle, Self + Offsets::LocalPlayer::PlayerMouse);
	return MouseClass(Handle, mousePtr);
}

CharacterClass PlayerClass::Character() {
	auto characterModelPtr = Memory::Read<uintptr_t>(Handle, Self + Offsets::LocalPlayer::ModelInstance);
	return ModelClass(Handle, characterModelPtr);
}

int PlayerClass::CharacterAppearanceId() {
	auto number = Memory::Read<int>(Handle, Self + Offsets::LocalPlayer::CharacterAppearanceId);
	return number;
}