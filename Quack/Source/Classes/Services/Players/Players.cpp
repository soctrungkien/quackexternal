#include "Players.hpp"

PlayerClass PlayersClass::LocalPlayer(const int timeout) {
	int startTime = GetTickCount();
	auto player = PlayerClass(Handle, Memory::Read<uintptr_t>(Handle, Self + Offsets::LocalPlayer::Pointer));
	while (!player.IsValid() && GetTickCount() - startTime < timeout) {
		player = PlayerClass(Handle, Memory::Read<uintptr_t>(Handle, Self + Offsets::LocalPlayer::Pointer));
		Sleep(5);
	}
	return player;
}