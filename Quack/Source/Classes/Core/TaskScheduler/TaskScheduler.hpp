#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class TaskSchedulerClass {
public:
    HANDLE Handle;
    uintptr_t Self;

    TaskSchedulerClass() = default;
    TaskSchedulerClass(HANDLE handle, uintptr_t address) :
        Handle(handle),
        Self(address)
    {}

    bool IsValid() const;
    static TaskSchedulerClass FetchTaskScheduler(HANDLE Handle, uintptr_t Address);
    std::vector<TaskSchedulerClass> GetJobs();

    uintptr_t GetJobStart();
    uintptr_t GetJobEnd();
    std::string GetJobName();
    TaskSchedulerClass GetJobByName(const std::string name);

    bool RemoveTaskByName(const std::string name);
    void UpdateJobPriority(uintptr_t jobAddress, int priority);
    void PauseTask(uintptr_t jobAddress);
    void ResumeTask(uintptr_t jobAddress);
    void PrintJobs();
    uintptr_t GetScriptContext();
    uintptr_t GetLuaState();

    float TargetFPS();
    void TargetFPS(float value);
    bool IsLoaded() { return this->GetJobs().size() > 0; }
};