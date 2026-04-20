#include "Teleport.hpp"
#include "Client/Client.hpp"

void Teleport_Loop(Client* client) {
    while (true) {
        if (!client->Is_Alive()) {
            client->Initialized = false;
            client->Cleanup(2);
            return;
        }

        if (client->DataModel.IsValid() && client->DataModel.Name() != "LuaApp" && client->DataModel.Name() != "Ugc") {
            while (client->Is_Alive()) {
                client->DataModel = DataModelClass::FetchDataModel(client->Handle, client->BaseAddress);
                if (client->DataModel.Name() == "LuaApp") {
                    if (Executor::DebugMode) std::cout << "[!] Player Left The Game" << std::endl;
                    client->Initialized = false;
                    client->Cleanup(1);
                    return;
                }
                else if (client->DataModel.Name() == "Ugc") {
                    if (Executor::DebugMode) std::cout << "[!] Player Rejoined" << std::endl;
                    client->Initialized = false;
                    client->Cleanup(1);
                    client->Attach();
                    return;
                }
            }
        }
        Sleep(50);
    }
}

void Teleport::Initialize(Client* client) {
    std::thread(Teleport_Loop, client).detach();
}