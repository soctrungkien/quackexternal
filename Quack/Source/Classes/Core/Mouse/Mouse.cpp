#include "Mouse.hpp"

Vector2 MouseClass::Position() {
	return Memory::Read<Vector2>(Handle, Self + Offsets::MouseService::MousePosition);
}