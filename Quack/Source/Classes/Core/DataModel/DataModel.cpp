#include "DataModel.hpp"

#include <iostream>

DataModelClass DataModelClass::FetchDataModel(HANDLE handle, uintptr_t address) {
	auto fake_datamodel_ptr = Memory::Read<uintptr_t>(handle, address + Offsets::DataModel::FakeDataModel);
	auto real_datamodel_ptr = Memory::Read<uintptr_t>(handle, fake_datamodel_ptr + Offsets::DataModel::RealDataModel);
	std::cout << "[Offsets] DataModel pointers: fake=0x" << std::hex << fake_datamodel_ptr
		<< " real=0x" << real_datamodel_ptr << std::dec << std::endl;
	return DataModelClass(handle, real_datamodel_ptr);
}

Instance DataModelClass::GetService(std::string name) {
	for (auto& child : GetChildren()) {
		if (child.ClassName() == name) {
			return child;
		}
	}
	return Instance();
}
