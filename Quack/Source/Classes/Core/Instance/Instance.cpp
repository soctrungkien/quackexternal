#include "Instance.hpp"

bool Instance::IsValid() const {
	return Self != 0;
}

uintptr_t Instance::This() {
	uintptr_t self_ptr = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::This);
	return self_ptr;
}

void Instance::This(uintptr_t new_this) {
	Memory::Write<uintptr_t>(Handle, Self + Offsets::Instance::This, new_this);
}

std::string Instance::ClassName() const {
	auto classDescriptor = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::ClassDescriptor);
	auto classNamePtr = Memory::Read<uintptr_t>(Handle, classDescriptor + Offsets::Instance::ClassName);

	auto stringSize = Memory::Read<size_t>(Handle, classNamePtr + 0x10);
	if (stringSize >= 16) {
		classNamePtr = Memory::Read<uintptr_t>(Handle, classNamePtr);
	}

	std::string className;
	uint8_t currentChar = 0;

	for (size_t offset = 0; (currentChar = Memory::Read<uint8_t>(Handle, classNamePtr + offset)) != 0; ++offset) {
		className.push_back(currentChar);
	}
	return className;
}

std::string Instance::Name() const {
	auto nameptr = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::Name);
	return Memory::Read_String(Handle, nameptr);
}

bool Instance::IsA(std::string class_name) {
	auto class_descriptor = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::ClassDescriptor);
	auto class_name_ptr = Memory::Read<uintptr_t>(Handle, class_descriptor + Offsets::Instance::ClassName);
	std::string class_name_str = Memory::Read_String(Handle, class_name_ptr);

	if (class_name_str == class_name) {
		return true;
	}

	while (true) {
		class_descriptor = Memory::Read<uintptr_t>(Handle, class_descriptor + Offsets::Instance::ClassDescriptorBase);
		class_name_ptr = Memory::Read<uintptr_t>(Handle, class_descriptor + Offsets::Instance::ClassName);
		class_name_str = Memory::Read_String(Handle, class_name_ptr);
		
		if (class_descriptor == 0 || class_name_ptr == 0) {
			return false;
		}

		if (class_name_str == "<<<ROOT>>>" && class_name != "<<<ROOT>>>") {
			return false;
		}

		if (class_name_str == class_name) {
			return true;
		}
	}
	return false;
}

Instance Instance::Parent() const {
	auto parentPtr = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::Parent);
	return Instance(Handle, parentPtr);
}

void Instance::Parent(Instance parent) {
	//Memory::Write(Handle, Self + Offsets::Instance::Parent, parent.Self);
}

std::vector<Instance> Instance::GetChildren() const {
	std::vector<Instance> children;
	auto start = Memory::Read<uintptr_t>(Handle, Self + Offsets::Instance::ChildrenStart);
	auto end = Memory::Read<uintptr_t>(Handle, start + Offsets::Instance::ChildrenEnd);

	for (auto ptr = Memory::Read<uintptr_t>(Handle, start); ptr != end; ptr += 0x10) {
		children.emplace_back(Handle, Memory::Read<uintptr_t>(Handle, ptr));
	}
	return children;
}

std::vector<Instance> Instance::GetDescendants() const {
	std::vector<Instance> descendants;

	std::function<void(const Instance&)> collectDescendants;
	collectDescendants = [&](const Instance& current) {
		auto children = current.GetChildren();
		for (const auto& child : children) {
			if (child.IsValid()) {
				descendants.push_back(child);
				collectDescendants(child);
			}
		}
	};

	collectDescendants(*this);
	return descendants;
}

Instance Instance::WaitForChild(const std::string& name, const int timeout) const {
	int startTime = GetTickCount();
	Instance child = FindFirstChild(name);

	while (!child.IsValid() && GetTickCount() - startTime < timeout) {
		child = FindFirstChild(name);
		Sleep(5);
	}
	return child;
}

Instance Instance::FindFirstChild(const std::string& name) const {
	for (const auto& child : GetChildren()) {
		if (child.Name() == name) {
			return child;
		}
	}
	return Instance();
}

Instance Instance::FindFirstChildOfClass(const std::string& class_name) const {
	for (const auto& child : GetChildren()) {
		if (child.ClassName() == class_name) {
			return child;
		}
	}
	return Instance();
}

void Instance::Delete() {
	Memory::Write<uint8_t>(Handle, Self + Offsets::Instance::Deleter, 1);
}

void Instance::DeleteBack() {
	Memory::Write<uint8_t>(Handle, Self + Offsets::Instance::DeleterBack, 1);
}