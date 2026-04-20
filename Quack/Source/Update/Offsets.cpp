#include "Update/Offsets.hpp"
#include "Update/OffsetFetcher.hpp"

#include <cctype>
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Offsets {
namespace {
    struct OffsetEntry {
        std::string Name;
        std::uintptr_t* Value;
    };

    std::vector<OffsetEntry>& GetRegistry() {
        static std::vector<OffsetEntry> registry = [] {
            std::vector<OffsetEntry> entries;
            entries.reserve(80);
            auto add = [&entries](const char* identifier, std::uintptr_t* value) {
                entries.push_back({identifier, value});
            };

            add("DataModel::FakeDataModel", &DataModel::FakeDataModel);
            add("DataModel::RealDataModel", &DataModel::RealDataModel);
            add("DataModel::ServerIP", &DataModel::ServerIP);
            add("DataModel::ScriptContext", &DataModel::ScriptContext);
            add("DataModel::JobId", &DataModel::JobId);
            add("DataModel::ToRenderView1", &DataModel::ToRenderView1);
            add("DataModel::ToRenderView2", &DataModel::ToRenderView2);
            add("DataModel::ToRenderView3", &DataModel::ToRenderView3);
            add("FakeDataModel::Pointer", &DataModel::FakeDataModel);
            add("FakeDataModel::RealDataModel", &DataModel::RealDataModel);
            add("DataModel::Ping", &DataModel::Ping);
            add("DataModel::CreatorId", &DataModel::CreatorId);
            add("DataModel::GameId", &DataModel::GameId);
            add("DataModel::GameLoaded", &DataModel::GameLoaded);
            add("DataModel::PlaceId", &DataModel::PlaceId);
            add("DataModel::PlaceVersion", &DataModel::PlaceVersion);
            add("DataModel::PrimitiveCount", &DataModel::PrimitiveCount);
            add("DataModel::Workspace", &DataModel::Workspace);

            add("TaskScheduler::Pointer", &TaskScheduler::Pointer);
            add("TaskScheduler::JobStart", &TaskScheduler::JobStart);
            add("TaskScheduler::JobEnd", &TaskScheduler::JobEnd);
            add("TaskScheduler::JobName", &TaskScheduler::JobName);
            add("TaskScheduler::JobType", &TaskScheduler::JobType);
            add("TaskScheduler::ScriptContext", &TaskScheduler::ScriptContext);
            add("TaskScheduler::RequireBypass", &TaskScheduler::RequireBypass);
            add("TaskScheduler::TargetFPS", &TaskScheduler::TargetFPS);
            add("TaskScheduler::FakeDataModel", &TaskScheduler::FakeDataModel);
            add("TaskScheduler::DataModel", &TaskScheduler::DataModel);

            add("Workspace::DistributedGameTime", &Workspace::DistributedGameTime);
            add("Workspace::ReadOnlyGravity", &Workspace::ReadOnlyGravity);
            add("Workspace::Gravity", &Workspace::Gravity);
            add("Workspace::GravityContainer", &Workspace::GravityContainer);
            add("Workspace::PrimitivesPointer1", &Workspace::PrimitivesPointer1);
            add("Workspace::PrimitivesPointer2", &Workspace::PrimitivesPointer2);

            add("Instance::AttributeContainer", &Instance::AttributeContainer);
            add("Instance::AttributeList", &Instance::AttributeList);
            add("Instance::AttributeToNext", &Instance::AttributeToNext);
            add("Instance::AttributeToValue", &Instance::AttributeToValue);
            add("Instance::This", &Instance::This);
            add("Instance::ClassDescriptorBase", &Instance::ClassDescriptorBase);
            add("Instance::ClassDescriptor", &Instance::ClassDescriptor);
            add("Instance::ClassName", &Instance::ClassName);
            add("Instance::Name", &Instance::Name);
            add("Instance::Parent", &Instance::Parent);
            add("Instance::ChildrenStart", &Instance::ChildrenStart);
            add("Instance::ChildrenEnd", &Instance::ChildrenEnd);
            add("Instance::Deleter", &Instance::Deleter);
            add("Instance::DeleterBack", &Instance::DeleterBack);

            add("Misc::Adornee", &Misc::Adornee);
            add("Misc::StringLength", &Misc::StringLength);
            add("Misc::Value", &Misc::Value);

            add("LocalPlayer::Pointer", &LocalPlayer::Pointer);
            add("LocalPlayer::UserId", &LocalPlayer::UserId);
            add("LocalPlayer::DisplayName", &LocalPlayer::DisplayName);
            add("LocalPlayer::PlayerMouse", &LocalPlayer::PlayerMouse);
            add("LocalPlayer::ModelInstance", &LocalPlayer::ModelInstance);
            add("LocalPlayer::CharacterAppearanceId", &LocalPlayer::CharacterAppearanceId);
            add("LocalPlayer::ForceNewAFKDurationPointer", &LocalPlayer::ForceNewAFKDurationPointer);

            add("RunService::HeartbeatFPS", &RunService::HeartbeatFPS);
            add("RunService::HeartbeatTask", &RunService::HeartbeatTask);

            add("Bytecode::OpcodeLookupTable", &Bytecode::OpcodeLookupTable);
            add("Bytecode::Pointer", &Bytecode::Pointer);
            add("Bytecode::Size", &Bytecode::Size);

            add("BaseScript::RunContext", &BaseScript::RunContext);

            add("LocalScript::ByteCode", &LocalScript::ByteCode);
            add("LocalScript::Hash", &LocalScript::Hash);

            add("ModuleScript::IsCoreScript", &ModuleScript::IsCoreScript);
            add("ModuleScript::ModuleFlags", &ModuleScript::ModuleFlags);
            add("ModuleScript::ByteCode", &ModuleScript::ByteCode);
            add("ModuleScript::Hash", &ModuleScript::Hash);

            add("MouseService::InputObject", &MouseService::InputObject);
            add("MouseService::MousePosition", &MouseService::MousePosition);
            add("MouseService::SensitivityPointer", &MouseService::SensitivityPointer);

            add("Model::PrimaryPart", &Model::PrimaryPart);
            add("Model::Scale", &Model::Scale);

            add("FFLAGS::WebSocketServiceEnableClientCreation", &FFLAGS::WebSocketServiceEnableClientCreation);
            add("FFLAGS::EnableLoadModule", &FFLAGS::EnableLoadModule);
            add("FFLAGS::TaskSchedulerTargetFps", &FFLAGS::TaskSchedulerTargetFps);
            add("FFLAGS::LockViolationInstanceCrash", &FFLAGS::LockViolationInstanceCrash);
            add("FFLAGS::DataSenderMaxBandwidthBps", &FFLAGS::DataSenderMaxBandwidthBps);
            add("FFLAGS::DataSenderMaxJoinBandwidthBps", &FFLAGS::DataSenderMaxJoinBandwidthBps);
            add("FFLAGS::PhysicsSenderMaxBandwidthBps", &FFLAGS::PhysicsSenderMaxBandwidthBps);
            add("FFLAGS::PhysicsSenderMaxBandwidthBpsScaling", &FFLAGS::PhysicsSenderMaxBandwidthBpsScaling);
            add("FFLAGS::LargeDataSenderMaxBandwidthBps", &FFLAGS::LargeDataSenderMaxBandwidthBps);
            add("FFLAGS::LargeDataSenderMinBandwidthBps", &FFLAGS::LargeDataSenderMinBandwidthBps);
            add("FFLAGS::WorldStepMax", &FFLAGS::WorldStepMax);
            add("FFLAGS::WorldStepsOffsetAdjustRate", &FFLAGS::WorldStepsOffsetAdjustRate);
            add("FFLAGS::HangDetectionReport", &FFLAGS::HangDetectionReport);
            add("FFLAGS::HangDetectionForceCrash", &FFLAGS::HangDetectionForceCrash);
            add("FFLAGS::DebugDisableTimeoutDisconnect", &FFLAGS::DebugDisableTimeoutDisconnect);

            return entries;
        }();
        return registry;
    }

    std::string Trim(const std::string& value) {
        size_t start = value.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            return "";
        }
        size_t end = value.find_last_not_of(" \t\r\n");
        return value.substr(start, end - start + 1);
    }

    std::string BuildFullName(const std::vector<std::string>& namespaces, const std::string& variable) {
        std::string result;
        for (size_t i = 0; i < namespaces.size(); ++i) {
            const std::string& ns = namespaces[i];
            if (i == 0 && ns == "Offsets") {
                continue;
            }
            if (!result.empty()) {
                result += "::";
            }
            result += ns;
        }
        if (!result.empty()) {
            result += "::";
        }
        result += variable;
        return result;
    }
}

bool UpdateFromDefinition(const std::string& definition) {
    auto& registry = GetRegistry();
    std::unordered_map<std::string, std::uintptr_t*> lookup;
    lookup.reserve(registry.size());
    for (auto& entry : registry) {
        lookup[entry.Name] = entry.Value;
    }

    std::vector<std::string> namespaces;
    std::istringstream stream(definition);
    std::string line;
    bool updated = false;

    while (std::getline(stream, line)) {
        std::string trimmed = Trim(line);
        if (trimmed.empty()) {
            continue;
        }

        size_t commentPos = trimmed.find("//");
        if (commentPos != std::string::npos) {
            trimmed = Trim(trimmed.substr(0, commentPos));
            if (trimmed.empty()) {
                continue;
            }
        }

        if (trimmed.rfind("namespace ", 0) == 0) {
            size_t nameStart = std::string("namespace ").size();
            size_t nameEnd = trimmed.find_first_of(" {", nameStart);
            if (nameEnd == std::string::npos) {
                nameEnd = trimmed.size();
            }
            namespaces.push_back(trimmed.substr(nameStart, nameEnd - nameStart));
            continue;
        }

        if (trimmed.find('}') != std::string::npos && !namespaces.empty()) {
            namespaces.pop_back();
            size_t bracePos = trimmed.find('}');
            trimmed = Trim(trimmed.substr(bracePos + 1));
            if (trimmed.empty()) {
                continue;
            }
        }

        size_t typePos = trimmed.find("uintptr_t");
        size_t eqPos = trimmed.find('=', typePos);
        if (typePos == std::string::npos || eqPos == std::string::npos) {
            continue;
        }

        size_t nameStart = trimmed.find_first_not_of(" \t", typePos + std::string("uintptr_t").size());
        size_t nameEnd = trimmed.find_first_of(" =\t", nameStart);
        if (nameStart == std::string::npos || nameEnd == std::string::npos) {
            continue;
        }

        std::string variable = trimmed.substr(nameStart, nameEnd - nameStart);
        size_t valueStart = trimmed.find("0x", nameEnd);
        if (valueStart == std::string::npos) {
            continue;
        }

        size_t valueEnd = trimmed.find_first_of(" ;\t", valueStart);
        std::string valueString = trimmed.substr(valueStart, valueEnd - valueStart);
        std::uintptr_t value = 0;
        try {
            value = static_cast<std::uintptr_t>(std::stoull(valueString, nullptr, 16));
        } catch (...) {
            continue;
        }

        std::string fullName = BuildFullName(namespaces, variable);
        auto it = lookup.find(fullName);
        if (it != lookup.end()) {
            if (value == 0) {
                std::cout << "[Offsets] Skipped " << fullName << " = 0x0 from remote (sentinel for missing-from-dump); keeping local 0x"
                          << std::hex << *(it->second) << std::dec << std::endl;
                continue;
            }
            *(it->second) = value;
            updated = true;
        }
    }

    return updated;
}

bool RefreshFromRemote() {
    bool updated = false;
    std::string payload;
    if (OffsetFetcher::DownloadText(L"https://imtheo.lol/Offsets/Offsets.hpp", payload)) {
        std::cout << "[Offsets] Downloaded Offsets.hpp (" << payload.size() << " bytes)" << std::endl;
        updated |= UpdateFromDefinition(payload);
    } else {
        std::cout << "[Offsets] Failed to download Offsets.hpp" << std::endl;
    }
    if (OffsetFetcher::DownloadText(L"https://imtheo.lol/Offsets/FFlags.hpp", payload)) {
        std::cout << "[Offsets] Downloaded FFlags.hpp (" << payload.size() << " bytes)" << std::endl;
        updated |= UpdateFromDefinition(payload);
    } else {
        std::cout << "[Offsets] Failed to download FFlags.hpp" << std::endl;
    }
    return updated;
}
} // namespace Offsets
