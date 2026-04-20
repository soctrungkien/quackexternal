#pragma once
#include <thread>
#include <functional>
#include "Update/Offsets.hpp"
#include "Memory/Memory.hpp"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"
#include "Math/Color.hpp"

class Instance {
public:
	HANDLE Handle;
	uintptr_t Self;

	Instance() = default;
	Instance(HANDLE handle, uintptr_t address) :
		Handle(handle),
		Self(address)
	{}

	bool IsValid() const;
	uintptr_t This();
	void This(uintptr_t new_this);
	std::string ClassName() const;
	std::string Name() const;
	bool IsA(std::string class_name);
	Instance Parent() const;
	void Parent(Instance parent);
	std::vector<Instance> GetChildren() const;
	std::vector<Instance> GetDescendants() const;
	Instance WaitForChild(const std::string& name, const int timeout = 100000) const;
	Instance FindFirstChild(const std::string& name) const;
	Instance FindFirstChildOfClass(const std::string& class_name) const;
	void Delete();
	void DeleteBack();
};