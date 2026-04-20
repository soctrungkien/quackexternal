#include "ModuleScript.hpp"
#include "Bytecode/Bytecode.hpp"
#include "SHA/SHA384.h"

inline SHA384 sha384;

std::string ModuleScriptClass::GetBytecode() {
	uintptr_t embedded = Memory::Read<uintptr_t>(Handle, Self + Offsets::ModuleScript::ByteCode);
	uintptr_t bytecode_ptr = Memory::Read<uintptr_t>(Handle, embedded + Offsets::Bytecode::Pointer);
	uint64_t bytecode_size = Memory::Read<uint64_t>(Handle, embedded + Offsets::Bytecode::Size);

	std::string bytecode_buffer;
	bytecode_buffer.resize(static_cast<size_t>(bytecode_size));

	ReadProcessMemory(Handle, reinterpret_cast<LPCVOID>(bytecode_ptr), bytecode_buffer.data(), static_cast<SIZE_T>(bytecode_size), NULL);
	return Bytecode::Decompress(bytecode_buffer);
}

void ModuleScriptClass::SetBytecode(const std::string& bytecode, bool revert) {
	if (!IsValid()) return;

	uintptr_t embedded = Memory::Read<uintptr_t>(Handle, Self + Offsets::ModuleScript::ByteCode);
	if (!embedded) return;

	LPVOID new_mem = VirtualAllocEx(Handle, NULL, bytecode.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!new_mem) {
		return;
	}
	DWORD old_protection;
	SIZE_T bytesWritten;

	BOOL writeSuccess = WriteProcessMemory(Handle, new_mem, bytecode.data(), bytecode.size(), &bytesWritten);
	if (!writeSuccess || bytesWritten != bytecode.size()) {
		VirtualFreeEx(Handle, new_mem, 0, MEM_RELEASE);
		return;
	}

	if (revert) {
		uintptr_t original_bytecode_ptr = Memory::Read<uintptr_t>(Handle, embedded + Offsets::Bytecode::Pointer);
		uint64_t original_size = Memory::Read<uint64_t>(Handle, embedded + Offsets::Bytecode::Size);

		std::thread([handle = this->Handle, new_mem, bytecode, old_protection, embedded, original_bytecode_ptr, original_size]() {
			Sleep(850);
			Memory::Write<uintptr_t>(handle, embedded + Offsets::Bytecode::Pointer, original_bytecode_ptr);
			Memory::Write<uint64_t>(handle, embedded + Offsets::Bytecode::Size, original_size);
			VirtualFreeEx(handle, new_mem, 0, MEM_RELEASE);
		}).detach();
	}

	Memory::Write<uintptr_t>(Handle, embedded + Offsets::Bytecode::Pointer, reinterpret_cast<uintptr_t>(new_mem));
	Memory::Write<uint64_t>(Handle, embedded + Offsets::Bytecode::Size, static_cast<uint64_t>(bytecode.size()));
}

std::string ModuleScriptClass::GetHash() {
	uintptr_t embedded = Memory::Read<uintptr_t>(Handle, Self + Offsets::ModuleScript::ByteCode);
	uintptr_t bytecode_ptr = Memory::Read<uintptr_t>(Handle, embedded + Offsets::Bytecode::Pointer);
	uint64_t bytecode_size = Memory::Read<uint64_t>(Handle, embedded + Offsets::Bytecode::Size);

	std::string bytecodeBuffer;
	bytecodeBuffer.resize(static_cast<size_t>(bytecode_size));

	SIZE_T bytesRead = 0;
	ReadProcessMemory(Handle, reinterpret_cast<LPCVOID>(bytecode_ptr), bytecodeBuffer.data(), static_cast<SIZE_T>(bytecode_size), &bytesRead);
	return sha384.hash(bytecodeBuffer);
}

uintptr_t ModuleScriptClass::IsCoreScript() {
	uintptr_t is_core_script = Memory::Read<uintptr_t>(Handle, Self + Offsets::ModuleScript::IsCoreScript);
	return is_core_script;
}

void ModuleScriptClass::IsCoreScript(Offsets::Enums::ModuleType type) {
	Memory::Write<uintptr_t>(Handle, Self + Offsets::ModuleScript::ModuleFlags, 0x100000000);
	Memory::Write<uintptr_t>(Handle, Self + Offsets::ModuleScript::IsCoreScript, type);
}

int ModuleScriptClass::RunContext() {
	int run_context = Memory::Read<int>(Handle, Self + Offsets::BaseScript::RunContext);
	return run_context;
}

void ModuleScriptClass::RunContext(Offsets::Enums::RunContext context) {
	Memory::Write<int>(Handle, Self + Offsets::BaseScript::RunContext, context);
}