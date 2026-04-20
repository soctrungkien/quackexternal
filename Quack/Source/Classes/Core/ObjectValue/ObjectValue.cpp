#include "ObjectValue.hpp"

Instance ObjectValueClass::Value() const {
	auto valueptr = Memory::Read<uintptr_t>(Handle, Self + Offsets::Misc::Value);
	return Instance(Handle, valueptr);
}