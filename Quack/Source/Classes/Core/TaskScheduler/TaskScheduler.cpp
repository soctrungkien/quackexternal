#include "TaskScheduler.hpp"
#include "Update/Offsets.hpp"
#include "Memory/Memory.hpp"

bool TaskSchedulerClass::IsValid() const {
    return Self != 0;
}

TaskSchedulerClass TaskSchedulerClass::FetchTaskScheduler(HANDLE handle, uintptr_t address) {
    uintptr_t pointer = Memory::Read<uintptr_t>(handle, address + Offsets::TaskScheduler::Pointer);
    return TaskSchedulerClass(handle, pointer);
}

std::vector<TaskSchedulerClass> TaskSchedulerClass::GetJobs() {
    std::vector<TaskSchedulerClass> jobs;
    uintptr_t start = Memory::Read<uintptr_t>(Handle, Self + Offsets::TaskScheduler::JobStart);
    uintptr_t end = Memory::Read<uintptr_t>(Handle, Self + Offsets::TaskScheduler::JobEnd);

    for (uintptr_t address = start; address < end; address += 0x10) {
        TaskSchedulerClass job = TaskSchedulerClass(Handle, Memory::Read<uintptr_t>(Handle, address));
        if (address && !job.GetJobName().empty()) {
            jobs.push_back(job);
        }
    }
    return jobs;
}

uintptr_t TaskSchedulerClass::GetJobStart() {
    return Memory::Read<uintptr_t>(Handle, Self + Offsets::TaskScheduler::JobStart);
}

uintptr_t TaskSchedulerClass::GetJobEnd() {
    return Memory::Read<uintptr_t>(Handle, Self + Offsets::TaskScheduler::JobEnd);
}

std::string TaskSchedulerClass::GetJobName() {
    uintptr_t namePtr = (Self + Offsets::TaskScheduler::JobName);
    int stringLength = Memory::Read<int>(Handle, namePtr + 0x18);

    if (stringLength >= 16)
        namePtr = Memory::Read<uintptr_t>(Handle, namePtr);

    std::vector<char> buffer(256);
    Memory::Read_Physical(Handle, namePtr, buffer.data(), buffer.size());
    return std::string(buffer.data());
}

TaskSchedulerClass TaskSchedulerClass::GetJobByName(const std::string name) {
    for (auto job : GetJobs()) {
        if (job.GetJobName() == name) {
            return job;
        }
    }
    return TaskSchedulerClass();
}

bool TaskSchedulerClass::RemoveTaskByName(const std::string name) {
    TaskSchedulerClass job = GetJobByName(name);
    if (!job.IsValid()) {
        return false;
    }

    Memory::Write<uint8_t>(Handle, job.Self + Offsets::Instance::Deleter, 1);
    return true;
}

void TaskSchedulerClass::UpdateJobPriority(uintptr_t jobAddress, int priority) {
    if (!jobAddress) return;
    //Memory::Write<uint8_t>(handle, jobAddress + Offsets::TaskScheduler::Priority, priority);
}

void TaskSchedulerClass::PauseTask(uintptr_t jobAddress) {
    if (!jobAddress) return;
    //Memory::Write<bool>(handle, jobAddress + Offsets::TaskScheduler::Interpation, true);
}

void TaskSchedulerClass::ResumeTask(uintptr_t jobAddress) {
    if (!jobAddress) return;
    //Memory::Write<bool>(handle, jobAddress + Offsets::TaskScheduler::Interpation, false);
}

void TaskSchedulerClass::PrintJobs() {
    auto jobs = GetJobs();
    uintptr_t start = GetJobStart();
    uintptr_t end = GetJobEnd();

    printf("\n");
    printf("==== Task Scheduler Jobs Start ====\n");
    for (auto job : jobs) {
        std::string name = job.GetJobName();
        if (name.empty()) {
            continue;
        }
        printf("=== Jobs -> %s ===\n", name.c_str());
    }

    printf("==== Task Scheduler Jobs End ====\n");
    printf("Job Range: 0x%p - 0x%p\n", (void*)start, (void*)end);
    printf("Total Jobs: %zu\n", jobs.size());
    printf("\n");
}


uintptr_t TaskSchedulerClass::GetScriptContext() {
    uintptr_t scriptsJob = GetJobByName("WaitingHybridScriptsJob").Self;
    uintptr_t scriptContext = Memory::Read<uintptr_t>(Handle, scriptsJob + Offsets::TaskScheduler::ScriptContext);
    return scriptContext;
}

uintptr_t TaskSchedulerClass::GetLuaState() {
    uintptr_t scriptContext = GetScriptContext();
    uintptr_t encodedStateAddress = scriptContext + 0x210;
    uintptr_t encodedState = Memory::Read<uintptr_t>(Handle, encodedStateAddress);

    uint32_t addressKey = (uint32_t)encodedStateAddress;
    uint64_t xorKey = ((uint64_t)addressKey) | (((uint64_t)addressKey) << 32);

    uintptr_t luaState = encodedState ^ xorKey;
    return luaState;
}

float TaskSchedulerClass::TargetFPS() {
    return Memory::Read<float>(Handle, Self + Offsets::TaskScheduler::TargetFPS);
}

void TaskSchedulerClass::TargetFPS(float value) {
    Memory::Write<float>(Handle, Self + Offsets::TaskScheduler::TargetFPS, value);
}