#include <iostream>
#include "nlohmann/json.hpp"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXWebSocketServer.h"
#include "WebsocketServer.hpp"

int port = 8080;
std::string host = "127.0.0.1";
ix::WebSocketServer Server(port, host);
std::map<int, ix::WebSocket*> Web_Socket_Clients;

void Start_Websocket_Server() {
    Server.setOnClientMessageCallback([](std::shared_ptr<ix::ConnectionState> connection_state, ix::WebSocket& web_socket, const ix::WebSocketMessagePtr& msg) {
        int client_id = std::stoi(connection_state->getId());

        if (msg->type == ix::WebSocketMessageType::Open) {
            Web_Socket_Clients[client_id] = &web_socket;
        }
        else if (msg->type == ix::WebSocketMessageType::Close) {
            Web_Socket_Clients.erase(client_id);
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {
            auto json_data = nlohmann::json::parse(msg->str);
            std::string pid = json_data.value("pid", "");
            std::string type = json_data.value("type", "");

            if (type == "remote_event") {
                std::string sub_type = json_data.value("sub_type", "");
                std::string event_id = json_data.value("event_id", "");

                if (sub_type == "create") {
                    std::string event_name = json_data.value("event_name", "");

                    for (auto& it : Web_Socket_Clients) {
                        auto& ws_id = it.first;
                        auto& ws = it.second;

                        if (ws_id == client_id) continue;

                        if (ws) {
                            nlohmann::json json_message = {
                                {"pid", pid},
                                {"type", type},
                                {"sub_type", sub_type},
                                {"success", true},
                                {"event_id", event_id},
                                {"event_name", event_name}
                            };
                            ws->send(json_message.dump());
                        }
                    }
                } else if (sub_type == "destroy") {
                    for (auto& it : Web_Socket_Clients) {
                        auto& ws_id = it.first;
                        auto& ws = it.second;

                        if (ws_id == client_id) continue;

                        if (ws) {
                            nlohmann::json json_message = {
                                {"pid", pid},
                                {"type", type},
                                {"sub_type", sub_type},
                                {"success", true},
                                {"event_id", event_id}
                            };
                            ws->send(json_message.dump());
                        }
                    }
                } else if (sub_type == "fire") {
                    std::string data = json_data.value("data", "");
                    std::cout << "Fire Data -> " << data << std::endl;

                    for (auto& it : Web_Socket_Clients) {
                        auto& ws_id = it.first;
                        auto& ws = it.second;

                        if (ws_id == client_id) continue;

                        if (ws) {
                            nlohmann::json json_message = {
                                {"pid", pid},
                                {"type", type},
                                {"sub_type", sub_type},
                                {"success", true},
                                {"event_id", event_id},
                                {"data", data},
                            };
                            ws->send(json_message.dump());
                        }
                    }
                }
            }
        }
    });

    auto res = Server.listen();
    if (!res.first) {
        std::cout << "[WEBSOCKET SERVER] Exception -> " << std::endl;
    }
    Server.start();
}