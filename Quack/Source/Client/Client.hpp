#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "Update/Offsets.hpp"
#include "Utility/Memory/Memory.hpp"
#include "Utility/Bytecode/Bytecode.hpp"
#include "Misc/Teleport/Teleport.hpp"
#include "ixwebsocket/IXWebSocket.h"
#include "DataModel/DataModel.hpp"
#include "TaskScheduler/TaskScheduler.hpp"
#include "Players/Players.hpp"
#include "Player/Player.hpp"
#include "Script/Script.hpp"
#include "ModuleScript/ModuleScript.hpp"
#include "ObjectValue/ObjectValue.hpp"

class Client {
public:
    DWORD PID;
    HANDLE Handle = NULL;
    uintptr_t BaseAddress = 0x0;
    DataModelClass DataModel = DataModelClass();
    TaskSchedulerClass TaskScheduler = TaskSchedulerClass();
    uintptr_t ScriptContext = 0x0;
    PlayerClass Player = PlayerClass();

    bool Initialized = false;
    bool Initializing = false;

    int CurrentExecutionID = 0;
    std::queue<std::string> Execution_Queue;
    std::vector<std::string> Teleport_Queue;
    std::filesystem::path ClientDirectory;
    std::string Version = "Unknown";
    std::string Username = "Unknown";
    ix::WebSocket* Web_Socket_Agent;
    std::map<int, ix::WebSocket*> Web_Socket_Agents;

    Client(DWORD PID) :
        PID(PID)
    {}

    bool Is_Alive();
    void Cleanup(int type);
    void Attach();
    Instance Get_Pointer_Instance(const std::string& pointer);
};