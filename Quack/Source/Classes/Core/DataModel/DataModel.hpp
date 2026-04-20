#pragma once
#include <string>
#include "Instance/Instance.hpp"

class DataModelClass : public Instance {
public:
	DataModelClass() = default;
	DataModelClass(HANDLE handle, uintptr_t address) :
		Instance(handle, address)
	{}

	DataModelClass(const Instance & instance) :
		Instance(instance.Handle, instance.Self)
	{}

	static DataModelClass FetchDataModel(HANDLE handle, uintptr_t address);
	Instance GetService(std::string name);
};