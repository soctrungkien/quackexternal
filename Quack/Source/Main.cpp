#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <thread>
#include <format>
#include <vector>
#include <shared_mutex>
#include "Utility/Bytecode/Bytecode.hpp"
#include "Server/WebsocketServer.hpp"
#include "Server/Server.hpp"
#include "Client/Client.hpp"
#include "Update/Offsets.hpp"

struct Client_Info {
    int PID;
    const char* Version;
    const char* Username;
};

typedef void(*Log_Callback)(int, const char*);
static Log_Callback log_callback = nullptr;

std::shared_mutex Clients_Mutex;
std::list<Client> Clients;

void Print(int type, const char* content) {
    if (log_callback) log_callback(type, content);
}

void _Initialize() {
    DWORD Current_Process = GetCurrentProcessId();
    std::vector<DWORD> Processes = Memory::Get_Processes("Quack");

    for (const DWORD& process : Processes) {
        if (process == Current_Process) {
            continue;
        }

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, process);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
        }
    }

    while (true) {
        {
            std::unique_lock lock(Clients_Mutex);
            auto PIDs = Memory::Get_Processes("Roblox");
            for (DWORD pid : PIDs) {
                bool exists = false;

                for (const auto& client : Clients) {
                    if (client.PID == pid) {
                        exists = true;
                    }
                }

                if (!exists) {
                    Clients.push_back(Client(pid));
                    if (Executor::DebugMode) std::cout << "[Quack] Added Client -> " << pid << std::endl;
                    Print(1, std::format("[Quack] Added Client -> {}", pid).c_str());
                }
                else {
                    //std::cout << "[-] Client Already Exists -> " << pid << std::endl;
                }
            }

            Clients.remove_if([](Client& client) {
                if (!client.Is_Alive()) {
                    if (Executor::DebugMode) std::cout << "[Quack] Removed 2 -> " << client.PID << std::endl;
                    Print(1, std::format("[Quack] Removed 2 -> {}", client.PID).c_str());
                    return true;
                }
                return false;
            });
        }
        Sleep(200);
    }
}

void _Attach() {
    std::unique_lock lock(Clients_Mutex);
    for (auto& client : Clients) {
        if (client.Is_Alive() && !client.Initializing && !client.Initialized) {
            client.Attach();
            if (Executor::DebugMode) std::cout << "[Quack] Attached 2 -> " << client.PID << std::endl;
            Print(1, std::format("[Quack] Attached 2 -> {}", client.PID).c_str());
        }
    }
}

void _Execute(std::string source) {
    std::unique_lock lock(Clients_Mutex);
    for (auto& client : Clients) {
        if (client.Is_Alive() && client.Initialized) {
            client.Execution_Queue.push(source);
            if (Executor::DebugMode) std::cout << "[Quack] I see u " << client.PID << std::endl;
        }
    }
}

std::string Convert(const wchar_t* wideStr) {
    if (!wideStr) return "";
    int size_needed = WideCharToMultiByte(
        CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr
    );
    if (size_needed == 0) return "";
    std::string result(size_needed, 0);
    WideCharToMultiByte(
        CP_UTF8, 0, wideStr, -1, &result[0], size_needed, nullptr, nullptr
    );
    if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }
    return result;
}

void Initialize() {
    Offsets::RefreshFromRemote();
    std::thread(Start_Websocket_Server).detach();
    std::thread(StartServer).detach();
    std::thread(_Initialize).detach();
}

void Attach() {
    std::thread(_Attach).detach();
}

void Execute(const std::string& source) {
    std::thread(_Execute, source).detach();
}

static bool IsRobloxRunning() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    bool found = false;
    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                found = true;
                break;
            }
        } while (Process32NextW(snapshot, &pe));
    }
    CloseHandle(snapshot);
    return found;
}

static bool HasInitializedClient() {
    std::unique_lock lock(Clients_Mutex);
    for (auto& client : Clients) {
        if (client.Is_Alive() && client.Initialized) {
            return true;
        }
    }
    return false;
}


int main() {
    std::cout << "[Quack] " << Executor::Version << " waiting for Roblox..." << std::endl;

    while (!IsRobloxRunning()) Sleep(1000);

    std::cout << "[Quack] Roblox detected" << std::endl;
    Initialize();

    bool greeted = false;
    while (true) {
        if (IsRobloxRunning()) {
            Attach();
            if (!greeted && HasInitializedClient()) {
                greeted = true;
                _Execute(std::string("print('ducknovis')"));
            }
        } else {
            if (!HasInitializedClient()) {
                std::cout << "[Quack] Roblox closed, exiting." << std::endl;
                break;
            }
        }
        Sleep(1000);
    }
    return 0;
}
