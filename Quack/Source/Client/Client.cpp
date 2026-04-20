#include "Client.hpp"

extern void Print(int type, const char* content);

bool Client::Is_Alive() {
    HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, PID);
    if (!handle) return false;

    DWORD exit_code = 0;
    BOOL ok = GetExitCodeProcess(handle, &exit_code);
    CloseHandle(handle);

    if (!ok) return false;
    return exit_code == STILL_ACTIVE;
}

void Client::Cleanup(int type) {
    if (type == 1) {
        Player = PlayerClass();

        if (Web_Socket_Agent) {
            Web_Socket_Agent->close();
        }

        if (!Web_Socket_Agents.empty()) {
            for (auto i = 1; i < Web_Socket_Agents.size(); i++) {
                Web_Socket_Agents[i]->close();
                Web_Socket_Agents.erase(i);
            }
        }
    }
    else if (type == 2) {
        PID = NULL;
        Handle = NULL;
        BaseAddress = 0x0;
        DataModel = DataModelClass();
        TaskScheduler = TaskSchedulerClass();
        ScriptContext = 0x0;
        Player = PlayerClass();

        if (Web_Socket_Agent) {
            Web_Socket_Agent->close();
        }

        if (!Web_Socket_Agents.empty()) {
            for (auto i = 1; i < Web_Socket_Agents.size(); i++) {
                Web_Socket_Agents[i]->close();
                Web_Socket_Agents.erase(i);
            }
        }
    }
}

void Client::Attach() {
    Initializing = true;
    if (Executor::DebugMode) std::cout << "----  Initializing Client -> " << PID << "  ----" << std::endl;

    if (Handle == NULL) {
        Handle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
        if (Handle == NULL) {
            std::cout << "[-] Failed To Allocate Handle To Roblox -> ??" << std::endl;
            Initializing = false;
            return;
        }
        std::cout << "[+] Allocated Handle To Roblox -> " << Handle << std::endl;
    }

    if (ClientDirectory.empty()) {
        ClientDirectory = Memory::Get_Path(Handle).parent_path();
    }

    if (Version.empty()) {
        Version = ClientDirectory.filename().string();
        std::cout << "[+] Found Roblox Version -> " << Version << std::endl;
    }

    if (!BaseAddress) {
        BaseAddress = Memory::Get_Base_Address(PID, L"RobloxPlayerBeta.exe");
        if (!BaseAddress) {
            std::cout << "[-] Roblox->BaseAddress Not Found -> ??" << std::endl;
            Initializing = false;
            return;
        }
        std::cout << "[+] Found Roblox Base Address -> 0x" << BaseAddress << std::endl;
    }

    if (!TaskScheduler.IsValid()) {
        TaskScheduler = TaskSchedulerClass::FetchTaskScheduler(Handle, BaseAddress);
        if (!TaskScheduler.IsValid()) {
            std::cout << "[-] Roblox->TaskScheduler Not Found -> ??" << std::endl;
            Initializing = false;
            return;
        }
        if (Executor::DebugMode) std::cout << "[+] Found Roblox Task Scheduler -> 0x" << TaskScheduler.Self << std::endl;
    }

    if (!DataModel.IsValid()) {
        while (Is_Alive() && !DataModel.IsValid()) {
            DataModel = DataModelClass::FetchDataModel(Handle, BaseAddress);
            if (!DataModel.IsValid()) {
                std::cout << "[-] Roblox->DataModel Not Found -> retrying..." << std::endl;
                Sleep(100);
            }
        }
        if (!DataModel.IsValid()) {
            Initializing = false;
            return;
        }
        if (Executor::DebugMode) std::cout << "[+] Found Roblox DataModel -> 0x" << DataModel.Self << std::endl;
    }

    if (ScriptContext == 0) {
        ScriptContext = TaskScheduler.GetScriptContext();
        if (ScriptContext == 0) {
            std::cout << "[-] TaskScheduler->ScriptContext Not Found -> ??" << std::endl;
            Initializing = false;
            return;
        }
        if (Executor::DebugMode) std::cout << "[+] Found Script Context -> 0x" << ScriptContext << std::endl;
    }

    while (Is_Alive() && DataModel.Name() != "Ugc") {
        DataModel = DataModelClass::FetchDataModel(Handle, BaseAddress);
        if (Executor::DebugMode) std::cout << "[Quack] Waiting For DataModel Ugc -> DataModel -> " << DataModel.Name() << std::endl;
        Sleep(50);
    }
    if (!Is_Alive()) {
        Initializing = false;
        return;
    }
    if (Executor::DebugMode && DataModel.Name() == "Ugc") std::cout << "[Quack] Found DataModel -> Ugc" << std::endl;

    // Player List Manager
    Instance CoreGui = DataModel.WaitForChild("CoreGui");
    if (!CoreGui.IsValid()) {
        std::cout << "[-] DataModel->CoreGui Not Found" << std::endl;
        Initializing = false;
        return;
    }

    Instance RobloxGui = CoreGui.WaitForChild("RobloxGui");
    if (!RobloxGui.IsValid()) {
        std::cout << "[-] CoreGui->RobloxGui Not Found" << std::endl;
        Initializing = false;
        return;
    }

    if (!RobloxGui.FindFirstChild("DropDownFullscreenFrame").IsValid()) {
        RobloxGui.WaitForChild("DropDownFullscreenFrame");
        if (DataModel.Name() != "Ugc") {
            return;
        }
        Sleep(2500);
    }

    PlayersClass Players = DataModel.GetService("Players");
    Player = Players.LocalPlayer();

    Username = Player.Name();
    while (Username == "Player" || Username.empty()) {
        Username = Player.Name();
        if (Executor::DebugMode) std::cout << "[!] Waiting For Player -> ??" << std::endl;
        Sleep(50);
    }
    if (Username == "Player" && Username.empty()) {
        Username = "Unknown";
        Initializing = false;
        return;
    }
    if (Executor::DebugMode) std::cout << "[+] Found Player -> " << Username << std::endl;

    Instance Modules = RobloxGui.WaitForChild("Modules");
    if (!Modules.IsValid()) {
        std::cout << "[-] RobloxGui->Modules Not Found" << std::endl;
        Initializing = false;
        return;
    }

    Instance PlayerList = Modules.WaitForChild("PlayerList");
    if (!PlayerList.IsValid()) {
        std::cout << "[-] Modules->PlayerList Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass PlayerListManager = PlayerList.WaitForChild("PlayerListManager");
    if (!PlayerListManager.IsValid()) {
        std::cout << "[-] PlayerList->PlayerListManager Not Found" << std::endl;
        Initializing = false;
        return;
    }

    // Jest Module
    Instance CorePackages = DataModel.WaitForChild("CorePackages");
    if (!CorePackages.IsValid()) {
        std::cout << "[-] DataModel->CorePackages Not Found" << std::endl;
        Initializing = false;
        return;
    }

    Instance Packages = CorePackages.WaitForChild("Packages");
    if (!Packages.IsValid()) {
        std::cout << "[-] CorePackages->Packages Not Found" << std::endl;
        Initializing = false;
        return;
    }

    Instance _Index = Packages.WaitForChild("_Index");
    if (!_Index.IsValid()) {
        std::cout << "[-] Packages->_Index Not Found" << std::endl;
        Initializing = false;
        return;
    }

    Instance CollisionMatchers2D = _Index.WaitForChild("CollisionMatchers2D");
    if (!CollisionMatchers2D.IsValid()) {
        std::cout << "[-] _Index->CollisionMatchers2D Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass CollisionMatchers2D_2 = CollisionMatchers2D.WaitForChild("CollisionMatchers2D");
    if (!CollisionMatchers2D_2.IsValid()) {
        std::cout << "[-] CollisionMatchers2D->CollisionMatchers2D Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass JestModule = CollisionMatchers2D_2.WaitForChild("Jest");
    if (!JestModule.IsValid()) {
        std::cout << "[-] CollisionMatchers2D_2->Jest Not Found" << std::endl;
        Initializing = false;
        return;
    }

    // Gamepad Module
    Instance PlayerScripts = Player.WaitForChild("PlayerScripts");
    if (!PlayerScripts.IsValid()) {
        std::cout << "[-] LocalPlayer->PlayerScripts Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass PlayerModule = PlayerScripts.WaitForChild("PlayerModule");
    if (!PlayerModule.IsValid()) {
        std::cout << "[-] PlayerScripts->PlayerModule Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass ControlModule = PlayerModule.WaitForChild("ControlModule");
    if (!ControlModule.IsValid()) {
        std::cout << "[-] PlayerModule->ControlModule Not Found" << std::endl;
        Initializing = false;
        return;
    }

    ModuleScriptClass GamepadModule = ControlModule.WaitForChild("Gamepad");
    if (!GamepadModule.IsValid()) {
        std::cout << "[-] ControlModule->GamepadModule Not Found" << std::endl;
        Initializing = false;
        return;
    }

    // Initialize Bytecode
    std::string InitScript = Resource::Get_Resource(255);
    std::string PatchScript = Resource::Get_Resource(254);

    Resource::Replace_String(InitScript, "%-PROCESS-ID-%", std::to_string(PID));
    Resource::Replace_String(InitScript, "%-QUACK-VERSION-%", Executor::Version);

    std::string signed_bytecode = Bytecode::Sign(Bytecode::Compile(InitScript));

    Memory::Write<uintptr_t>(Handle, BaseAddress + Offsets::FFLAGS::WebSocketServiceEnableClientCreation, 1);
    //Memory::Write<BYTE>(Handle, BaseAddress + Offsets::FFLAGS::EnableLoadModule, 1);
    //std::cout << "RequireBypass -> 0x" << Memory::Read<uintptr_t>(Handle, ScriptContext + Offsets::TaskScheduler::RequireBypass) << std::endl;
    //Memory::Write<uintptr_t>(Handle, ScriptContext + Offsets::TaskScheduler::RequireBypass, 0x1);
    //std::cout << "RequireBypass -> 0x" << Memory::Read<uintptr_t>(Handle, ScriptContext + Offsets::TaskScheduler::RequireBypass) << std::endl;

    if (Executor::DebugMode) std::cout << "[!] JestModule -> IsCoreScript -> 0x" << (int)JestModule.IsCoreScript() << std::endl;
    JestModule.IsCoreScript(Offsets::Enums::Core);
    if (Executor::DebugMode) std::cout << "[!] JestModule -> IsCoreScript -> 0x" << (int)JestModule.IsCoreScript() << std::endl;

    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> This -> 0x" << PlayerListManager.This() << std::endl;
    PlayerListManager.This(JestModule.Self);
    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> This -> 0x" << PlayerListManager.This() << std::endl;
    JestModule.SetBytecode(signed_bytecode, true);

    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> Macro -> Start" << std::endl;
    HWND hwnd = FindWindowA(NULL, "Roblox");
    HWND old = GetForegroundWindow();
    while (GetForegroundWindow() != hwnd) {
        SetForegroundWindow(hwnd);
    }
    keybd_event(VK_ESCAPE, MapVirtualKey(VK_ESCAPE, 0), KEYEVENTF_SCANCODE, 0);
    keybd_event(VK_ESCAPE, MapVirtualKey(VK_ESCAPE, 0), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
    Sleep(50);
    if (old != NULL) {
        SetForegroundWindow(old);
    }
    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> Macro -> End" << std::endl;

    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> This -> 0x" << PlayerListManager.This() << std::endl;
    PlayerListManager.This(PlayerListManager.Self);
    if (Executor::DebugMode) std::cout << "[!] PlayerListManager -> This -> 0x" << PlayerListManager.This() << std::endl;

    Instance bindable;
    while (!bindable.IsValid()) {
        bindable = JestModule.FindFirstChildOfClass("BindableEvent");
        Sleep(50);
    }
    Sleep(200);
    if (Executor::DebugMode) std::cout << "[+] found bindable_event @ 0x" << std::hex << bindable.Self << std::endl;

    auto signals = Memory::Read<uintptr_t>(Handle, bindable.Self + Offsets::BindableEvent::Signals);
    if (Executor::DebugMode) std::cout << "[+] found signals @ 0x" << std::hex << signals << std::endl;
    auto function_ptr = Memory::Read<uintptr_t>(Handle, signals + Offsets::BindableEvent::Function);
    if (Executor::DebugMode) std::cout << "[+] found function @ 0x" << std::hex << function_ptr << std::endl;
    auto function_impl = Memory::Read<uintptr_t>(Handle, function_ptr + Offsets::BindableEvent::FunctionImpl);
    if (Executor::DebugMode) std::cout << "[+] found function_impl @ 0x" << std::hex << function_impl << std::endl;
    auto thread_ref = Memory::Read<uintptr_t>(Handle, function_impl + Offsets::BindableEvent::ThreadRef);
    if (Executor::DebugMode) std::cout << "[+] found thread_ref @ 0x" << std::hex << thread_ref << std::endl;
    auto live_thread = Memory::Read<uintptr_t>(Handle, thread_ref + Offsets::BindableEvent::LiveThread);
    if (Executor::DebugMode) std::cout << "[+] found live_thread @ 0x" << std::hex << live_thread << std::endl;
    auto lua_state = Memory::Read<uintptr_t>(Handle, live_thread + Offsets::BindableEvent::LuaState);
    if (Executor::DebugMode) std::cout << "[+] found lua_state @ 0x" << std::hex << lua_state << std::endl;

    auto global_state = Memory::Read<uintptr_t>(Handle, lua_state + Offsets::LuaState::GlobalState);
    if (Executor::DebugMode) std::cout << "[+] found global_state @ 0x" << std::hex << global_state << std::endl;
    auto main_thread = Memory::Read<uintptr_t>(Handle, global_state + Offsets::GlobalState::MainThread);
    if (Executor::DebugMode) std::cout << "[+] found main_thread @ 0x" << std::hex << main_thread << std::endl;
    auto handler_userdata = Memory::Read<uintptr_t>(Handle, lua_state + Offsets::LuaState::Userdata);
    if (Executor::DebugMode) std::cout << "[+] found handler_userdata @ 0x" << std::hex << handler_userdata << std::endl;
    auto main_userdata = Memory::Read<uintptr_t>(Handle, main_thread + Offsets::LuaState::Userdata);
    if (!main_userdata) {
        main_userdata = handler_userdata;
        if (Executor::DebugMode) std::cout << "[!] main_userdata was null, falling back to handler_userdata" << std::endl;
    }
    if (Executor::DebugMode) std::cout << "[+] found main_userdata @ 0x" << std::hex << main_userdata << std::endl;

    int32_t identity = 8;
    uintptr_t caps = identity_to_caps(identity);

    Memory::Write<int32_t>(Handle, main_userdata + Offsets::ExtraSpace::Identity, identity);
    Memory::Write<uintptr_t>(Handle, main_userdata + Offsets::ExtraSpace::Caps, caps);

    Memory::Write<int32_t>(Handle, handler_userdata + Offsets::ExtraSpace::Identity, identity);
    Memory::Write<uintptr_t>(Handle, handler_userdata + Offsets::ExtraSpace::Caps, caps);

    std::cout << std::dec << "[+] set identity " << identity << " on main_thread and handler_thread" << std::endl;

    if (Executor::DebugMode) std::cout << "----  Client Initialized -> " << PID << "  ----" << std::endl;
    Initialized = true;
    Initializing = false;
    Teleport::Initialize(this);
}

Instance Client::Get_Pointer_Instance(const std::string& pointer) {
    Instance RobloxReplicatedStorage = DataModel.GetService("RobloxReplicatedStorage");
    if (!RobloxReplicatedStorage.IsValid()) {
        std::cout << "[-] DataModel->RobloxReplicatedStorage Not Found" << std::endl;
        return Instance();
    }

    Instance QuackContainer = RobloxReplicatedStorage.FindFirstChild("Quack");
    if (!QuackContainer.IsValid()) {
        std::cout << "[-] RobloxReplicatedStorage->Quack Not Found" << std::endl;
        return Instance();
    }

    Instance PointerContainer = QuackContainer.FindFirstChild("Pointers");
    if (!PointerContainer.IsValid()) {
        std::cout << "[-] QuackContainer->Pointers Not Found" << std::endl;
        return Instance();
    }

    ObjectValueClass Pointer = PointerContainer.FindFirstChild(pointer);
    if (!Pointer.IsValid()) {
        return Instance();
    }

    Instance Obj = Pointer.Value();
    if (!Obj.IsValid()) {
        return Instance();
    }
    return Obj;
}
