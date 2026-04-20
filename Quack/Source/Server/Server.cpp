#define CPPHTTPLIB_MBEDTLS_SUPPORT
#include <iostream>
#include <fstream>
#include <regex>
#include <list>
#include <shared_mutex>
#include <filesystem>
#include <type_traits>

#include "Server.hpp"
#include "base64/base64.hpp"
#include "httplib/httplib.h"
#include "nlohmann/json.hpp"
#include "Client/Client.hpp"
#include "Bytecode/Bytecode.hpp"
#include "mbedtls/md.h"

using namespace std;
using json = nlohmann::json;

extern std::shared_mutex Clients_Mutex;
extern std::list<Client> Clients;

bool Console_Active = false;
std::mutex Console_Mutex;
void CreateConsole() {
    std::lock_guard<std::mutex> lock(Console_Mutex);
    if (Console_Active) return;
    Console_Active = true;

    AllocConsole();
    SetConsoleTitleA("Quack");

    FILE* fOut;
    FILE* fIn;
    FILE* fErr;

    freopen_s(&fOut, "CONOUT$", "w", stdout);
    freopen_s(&fIn, "CONIN$", "r", stdin);
    freopen_s(&fErr, "CONOUT$", "w", stderr);
    std::ios::sync_with_stdio(true);

    std::cout.clear();
    std::cerr.clear();
    std::wcout.clear();
    std::wcerr.clear();
}

void StartServer() {
    if (Executor::DebugMode) {
        CreateConsole();
    }
    httplib::Server Http_Server;

    std::filesystem::path Main_Directory;
    std::filesystem::path Auto_Execute_Directory;
    std::filesystem::path Scripts_Directory;
    std::filesystem::path Workspace_Directory;

    char Main_Path[MAX_PATH];
    GetModuleFileNameA(NULL, Main_Path, MAX_PATH);

    Main_Directory = std::filesystem::path(Main_Path).parent_path();
    Auto_Execute_Directory = Main_Directory / "autoexecute";
    Scripts_Directory = Main_Directory / "scripts";
    Workspace_Directory = Main_Directory / "workspace";

    if (!std::filesystem::is_directory(Auto_Execute_Directory)) {
        std::filesystem::create_directory(Auto_Execute_Directory);
    }

    if (!std::filesystem::is_directory(Scripts_Directory)) {
        std::filesystem::create_directory(Scripts_Directory);
    }

    if (!std::filesystem::is_directory(Workspace_Directory)) {
        std::filesystem::create_directory(Workspace_Directory);
    }

    // Filesystem
    Http_Server.Post("/delfile", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted To Escape Directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            if (!std::filesystem::exists(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "File Does Not Exist"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::remove(targetPath);

            nlohmann::json response = {
                {"success", true},
                {"message", "File Deleted Successfully"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/delfolder", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted To Escape Directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            if (!std::filesystem::exists(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Folder Does Not Exist"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::filesystem::remove_all(targetPath);

            nlohmann::json response = {
                {"success", true},
                {"message", "Folder Deleted Successfully"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/getcustomasset", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string path = json_data.value("path", "");

            if (path.empty() || pid.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Attempted to escape directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::filesystem::path targetPath = Workspace_Directory / path;

            if (!std::filesystem::exists(targetPath)) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "File Does Not Exist"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            if (!std::filesystem::is_regular_file(targetPath)) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Path Is Not A File"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    std::filesystem::path contentDirectory = client.ClientDirectory / "content";

                    if (!std::filesystem::exists(contentDirectory)) {
                        std::filesystem::create_directories(contentDirectory);
                    }
                    std::filesystem::path destinationPath = contentDirectory / targetPath.filename();

                    try {
                        std::filesystem::copy_file(targetPath, destinationPath, std::filesystem::copy_options::overwrite_existing);
                        std::string assetId = "rbxasset://" + targetPath.filename().string();

                        nlohmann::json response = {
                            {"success", true},
                            {"data", assetId}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    catch (const std::exception& e) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Failed To Copy File: " + std::string(e.what())}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/isfile", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted to escape directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            nlohmann::json response = {
                {"success", true},
                {"data", std::filesystem::is_regular_file(targetPath)},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/isfolder", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted to escape directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            nlohmann::json response = {
                {"success", true},
                {"data", std::filesystem::is_directory(targetPath)},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/listfiles", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted to escape directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            if (!std::filesystem::exists(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Directory Doesn't Exist"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            nlohmann::json filesArray = nlohmann::json::array();
            for (auto& entry : std::filesystem::directory_iterator(targetPath)) {
                std::filesystem::path relativePath = std::filesystem::relative(entry.path(), Workspace_Directory);
                filesArray.push_back(relativePath.generic_string());
            }

            nlohmann::json response = {
                {"success", true},
                {"data", filesArray}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/makefolder", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted To Escape Directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;
            bool created = std::filesystem::create_directories(targetPath);

            nlohmann::json response = {
                {"success", true},
                {"message", created ? "Folder Created Successfully" : "Folder Already Exists"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/readfile", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted To Escape Directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            std::filesystem::path targetPath = Workspace_Directory / path;

            if (!std::filesystem::exists(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "File Does Not Exist: " + path}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            if (std::filesystem::is_directory(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Path Is A Directory, Not A File: " + path}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            if (!std::filesystem::is_regular_file(targetPath)) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Path Is Not A Regular File: " + path}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            auto fileSize = std::filesystem::file_size(targetPath);
            if (fileSize == 0) {
                nlohmann::json response = {
                    {"success", true},
                    {"data", base64::to_base64("")}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::ifstream file(targetPath, std::ios::binary);
            if (!file.is_open()) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Failed To Open File: " + path}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            content = base64::to_base64(content);
            file.close();
            nlohmann::json response = {
                {"success", true},
                {"data", content},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::filesystem::filesystem_error& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Filesystem Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/writefile", [Workspace_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string path = json_data.value("path", "");
            std::string data = json_data.value("data", "");

            if (path.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: path"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            data = base64::from_base64(data);

            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.find("..") != std::string::npos) {
                nlohmann::json response = {
                   {"success", false},
                   {"error", "Attempted To Escape Directory"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::filesystem::path targetPath = Workspace_Directory / path;
            std::filesystem::create_directories(targetPath.parent_path());

            if (targetPath.string().length() > 260) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Path Length Exceeds System Limit"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::ofstream file(targetPath, std::ios::binary);
            if (!file) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Failed To Open File For Writing"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            file.write(data.data(), data.size());
            file.close();

            if (!file) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Error Occurred While Writing To File"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            nlohmann::json response = {
                {"success", true},
                {"message", "File Written Successfully"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Instances
    Http_Server.Post("/getaddress", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    Instance instance = client.Get_Pointer_Instance(pointer);
                    std::string address = std::to_string(instance.Self);

                    nlohmann::json response = {
                        {"success", true},
                        {"data", address},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/spoofinstance", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");
            std::string new_address_str = json_data.value("address", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            uintptr_t new_address = static_cast<uintptr_t>(stoull(new_address_str));

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    Instance instance = client.Get_Pointer_Instance(pointer);
                    instance.This(new_address);

                    nlohmann::json response = {
                        {"success", true},
                        {"message", "Instance Spoofed Successfully"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Miscellaneous
    Http_Server.Post("/request", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string url = json_data.value("l", "");
            std::string method = json_data.value("m", "");
            std::string rBody = json_data.value("b", "");
            json headersJ = json_data.value("h", nlohmann::json::object());
            rBody = base64::from_base64(rBody);

            if (url.empty() || method.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: url (l) or method (m)"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            if (url.find("http://") == std::string::npos && url.find("https://") == std::string::npos) {
                url = "https://" + url;
            }

            std::regex urlR(R"(^(http[s]?:\/\/)?([^\/:]+)(:\d+)?(\/.*)?$)");
            std::smatch urlM;
            std::string host;
            int port = -1;
            std::string path = "/";
            std::string protocol = "https";

            if (std::regex_match(url, urlM, urlR)) {
                if (urlM[1].matched) {
                    std::string proto = urlM[1];
                    if (proto.find("https") != std::string::npos) {
                        protocol = "https";
                    }
                    else {
                        protocol = "http";
                    }
                }

                host = urlM[2];
                if (urlM[4].matched) path = urlM[4];

                if (urlM[3].matched) {
                    // strip ':' and convert to int
                    std::string portStr = urlM[3].str();
                    portStr.erase(0, 1);
                    port = std::stoi(portStr);
                }
            }
            else {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Invalid URL Format: " + url}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            httplib::Headers headers;
            for (auto it = headersJ.begin(); it != headersJ.end(); ++it) {
                headers.insert({ it.key(), it.value() });
            }

            if (headers.find("User-Agent") == headers.end()) {
                headers.insert({ "User-Agent", "Quack/1.0" });
            }

            if (method != "GET" && method != "POST" && method != "PUT" && method != "DELETE" && method != "PATCH") {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Unsupported HTTP Method: " + method}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            auto perform_request = [&](auto& client) -> httplib::Result {
                client.set_follow_location(true);
                client.set_connection_timeout(20);
                client.set_read_timeout(120);
                if constexpr (std::is_same_v<std::decay_t<decltype(client)>, httplib::SSLClient>) {
                    client.enable_server_certificate_verification(false);
                }

                if (method == "GET") {
                    return client.Get(path, headers);
                }
                else if (method == "POST") {
                    std::string contentType = "application/json";
                    auto contentTypeIt = headers.find("Content-Type");
                    if (contentTypeIt != headers.end()) {
                        contentType = contentTypeIt->second;
                    }
                    return client.Post(path, headers, rBody, contentType);
                }
                else if (method == "PUT") {
                    std::string contentType = "application/json";
                    auto contentTypeIt = headers.find("Content-Type");
                    if (contentTypeIt != headers.end()) {
                        contentType = contentTypeIt->second;
                    }
                    return client.Put(path, headers, rBody, contentType);
                }
                else if (method == "DELETE") {
                    return client.Delete(path, headers);
                }
                else if (method == "PATCH") {
                    std::string contentType = "application/json";
                    auto contentTypeIt = headers.find("Content-Type");
                    if (contentTypeIt != headers.end()) {
                        contentType = contentTypeIt->second;
                    }
                    return client.Patch(path, headers, rBody, contentType);
                }
                return {}; // unreachable because method validated above
            };

            httplib::Result proxiedRes;
            if (protocol == "https") {
                if (port == -1) port = 443;
                httplib::SSLClient client(host.c_str(), port);
                proxiedRes = perform_request(client);
            }
            else {
                if (port == -1) port = 80;
                httplib::Client client(host.c_str(), port);
                proxiedRes = perform_request(client);
            }

            // proxiedRes may still be empty if client couldn't connect; handled below

            if (proxiedRes) {
                nlohmann::json responseJ;
                responseJ["success"] = true;
                responseJ["c"] = proxiedRes->status;
                responseJ["r"] = proxiedRes->reason;
                responseJ["v"] = proxiedRes->version;

                nlohmann::json rHeadersJ = nlohmann::json::object();
                for (const auto& header : proxiedRes->headers) {
                    rHeadersJ[header.first] = header.second;
                }
                responseJ["h"] = rHeadersJ;

                const auto contentType = proxiedRes->get_header_value("Content-Type");
                bool isTextContent =
                    contentType.find("application/json") != std::string::npos ||
                    contentType.find("text/") != std::string::npos ||
                    contentType.find("application/xml") != std::string::npos ||
                    contentType.find("application/javascript") != std::string::npos;

                // Roblox HttpService has body limits and struggles with very large text payloads.
                // For large responses, always base64 to keep JSON small and avoid invalid UTF-8.
                const bool largeBody = proxiedRes->body.size() > (512 * 1024); // 512 KB threshold
                if ((!isTextContent || largeBody) && !proxiedRes->body.empty()) {
                    responseJ["b"] = base64::to_base64(proxiedRes->body);
                    responseJ["b64"] = true;
                } else {
                    responseJ["b"] = proxiedRes->body;
                }

                res.set_content(responseJ.dump(), "application/json");
            }
            else {
                auto err = proxiedRes.error();
                nlohmann::json response = {
                    {"success", false},
                    {"error", "HTTP Request Failed: " + httplib::to_string(err)}
                };
                res.set_content(response.dump(), "application/json");
            }

        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/setclipboard", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string data = json_data.value("data", "");

            if (data.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: data"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            data = base64::from_base64(data);

            if (!OpenClipboard(nullptr)) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Unable To Open Clipboard"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);
            if (!hMem) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Unable To Allocate Memory For Clipboard"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            char* pMem = static_cast<char*>(GlobalLock(hMem));
            if (!pMem) {
                GlobalFree(hMem);
                CloseClipboard();
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Unable To Set Clipboard Data"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::copy(data.begin(), data.end(), pMem);
            pMem[data.size()] = '\0';
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
            CloseClipboard();

            nlohmann::json response = {
                {"success", true},
                {"message", "Clipboard Set Successfully"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Scripts
    Http_Server.Post("/cancompile", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string source = json_data.value("source", "");

            if (source.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: source"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            source = base64::from_base64(source);

            auto result = Bytecode::Compilable(source);
            if (result.Compiled) {
                nlohmann::json response = {
                    {"success", true},
                    {"data", true}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else {
                nlohmann::json response = {
                    {"success", true},
                    {"errors", result.Errors}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            nlohmann::json response = {
                {"success", true},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/compile", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string source = json_data.value("source", "");
            bool encoded = json_data.value("encoded", true);

            if (source.empty() || !json_data.contains("encoded")) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: source or encoded"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            source = base64::from_base64(source);

            if (encoded == true) {
                std::string bytecode = Bytecode::Compile(source);
                if (bytecode.empty()) {
                    nlohmann::json response = {
                        {"success", false},
                        {"error", "Compilation Failed"}
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                nlohmann::json response = {
                    {"success", true},
                    {"data", base64::to_base64(bytecode)},
                };
                res.set_content(response.dump(), "application/json");
            }
            else {
                std::string bytecode = Bytecode::NormalCompile(source);
                if (bytecode.empty()) {
                    nlohmann::json response = {
                        {"success", false},
                        {"error", "Compilation Failed"}
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                nlohmann::json response = {
                    {"success", true},
                    {"data", base64::to_base64(bytecode)},
                };
                res.set_content(response.dump(), "application/json");
            }
            return;
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/loadstring", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");
            std::string source = json_data.value("source", "");

            if (pid.empty() || pointer.empty() || source.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid, pointer or source"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            source = base64::from_base64(source);

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    Instance roblox_replicated_storage = client.DataModel.GetService("RobloxReplicatedStorage");
                    if (!roblox_replicated_storage.IsValid()) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Loadstring Failed"}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    Instance hyperion_container = roblox_replicated_storage.FindFirstChild("Quack");
                    if (!hyperion_container.IsValid()) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Loadstring Failed"}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    Instance loadstring_container = hyperion_container.FindFirstChild("Loadstring");
                    if (!loadstring_container.IsValid()) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Loadstring Failed"}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    ModuleScriptClass module = loadstring_container.FindFirstChild(pointer);
                    if (!module.IsValid()) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Loadstring Failed"}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    auto result = Bytecode::Compilable(source);
                    if (!result.Compiled) {
                        nlohmann::json response = {
                            {"success", false},
                            {"errors", result.Errors}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    std::string compiled_bytecode = Bytecode::Compile(source);
                    if (compiled_bytecode.size() <= 1) {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Compilation Failed"},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    std::string signed_bytecode = Bytecode::Sign(compiled_bytecode);
                    module.IsCoreScript(Offsets::Enums::ModuleType::Core);
                    module.SetBytecode(signed_bytecode, false);

                    nlohmann::json response = {
                        {"success", true},
                        {"message", "Loadstring Executed Successfully"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }
            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // /hash - mbedtls-backed message digest. Body: {algorithm, data, [encoding]}.
    // algorithm: md5 | sha1 | sha224 | sha256 | sha384 | sha512 | sha3-224 | sha3-256 | sha3-384 | sha3-512
    // data: base64-encoded input (so binary safe; lua side base64-encodes before sending)
    // encoding: "hex" (default) or "base64" for the returned digest
    Http_Server.Post("/hash", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string algorithm = json_data.value("algorithm", "");
            std::string data_b64  = json_data.value("data", "");
            std::string encoding  = json_data.value("encoding", "hex");

            // normalize algorithm: lowercase + strip "-" / "_"
            std::string norm;
            for (char c : algorithm) {
                if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                if (c == '-' || c == '_') continue;
                norm += c;
            }

            mbedtls_md_type_t md_type = MBEDTLS_MD_NONE;
            if      (norm == "md5")     md_type = MBEDTLS_MD_MD5;
            else if (norm == "sha1")    md_type = MBEDTLS_MD_SHA1;
            else if (norm == "sha224")  md_type = MBEDTLS_MD_SHA224;
            else if (norm == "sha256")  md_type = MBEDTLS_MD_SHA256;
            else if (norm == "sha384")  md_type = MBEDTLS_MD_SHA384;
            else if (norm == "sha512")  md_type = MBEDTLS_MD_SHA512;
            else if (norm == "sha3224") md_type = MBEDTLS_MD_SHA3_224;
            else if (norm == "sha3256") md_type = MBEDTLS_MD_SHA3_256;
            else if (norm == "sha3384") md_type = MBEDTLS_MD_SHA3_384;
            else if (norm == "sha3512") md_type = MBEDTLS_MD_SHA3_512;

            if (md_type == MBEDTLS_MD_NONE) {
                nlohmann::json response = {{"success", false}, {"error", "Unsupported algorithm: " + algorithm}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            const mbedtls_md_info_t* md_info = mbedtls_md_info_from_type(md_type);
            if (!md_info) {
                nlohmann::json response = {{"success", false}, {"error", "mbedtls_md_info_from_type returned null"}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::string raw = base64::from_base64(data_b64);
            unsigned char digest[64] = {0};
            int ret = mbedtls_md(md_info,
                                 reinterpret_cast<const unsigned char*>(raw.data()),
                                 raw.size(),
                                 digest);
            if (ret != 0) {
                nlohmann::json response = {{"success", false}, {"error", "mbedtls_md returned " + std::to_string(ret)}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            unsigned char out_size = mbedtls_md_get_size(md_info);
            std::string out;
            if (encoding == "base64") {
                out = base64::to_base64(std::string(reinterpret_cast<char*>(digest), out_size));
            } else {
                out.reserve(out_size * 2);
                static const char* hex = "0123456789abcdef";
                for (unsigned i = 0; i < out_size; ++i) {
                    out.push_back(hex[(digest[i] >> 4) & 0xF]);
                    out.push_back(hex[digest[i] & 0xF]);
                }
            }

            nlohmann::json response = {
                {"success", true},
                {"hash", out},
                {"algorithm", algorithm},
                {"size", static_cast<int>(out_size)}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {{"success", false}, {"error", "Server Error: " + std::string(e.what())}};
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/iscorescript", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    ModuleScriptClass script = client.Get_Pointer_Instance(pointer);
                    bool is_core_script = script.IsCoreScript() == 0x1;

                    nlohmann::json response = {
                        {"success", true},
                        {"data", is_core_script},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/unlockmodule", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    ModuleScriptClass script = client.Get_Pointer_Instance(pointer);
                    script.IsCoreScript(Offsets::Enums::ModuleType::Core);
                    nlohmann::json response = {
                        {"success", true},
                        {"message", "Unlocked Module Successfully"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/getscriptbytecode", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    Instance base_script = client.Get_Pointer_Instance(pointer);
                    if (base_script.ClassName() == "LocalScript" || base_script.ClassName() == "Script") {
                        ScriptClass script = base_script;
                        std::string bytes = script.GetBytecode();
                        std::string encoded_bytecode = base64::to_base64(bytes);

                        nlohmann::json response = {
                            {"success", true},
                            {"data", encoded_bytecode},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else if (base_script.ClassName() == "ModuleScript") {
                        ModuleScriptClass script = base_script;
                        std::string bytes = script.GetBytecode();
                        std::string encoded_bytecode = base64::to_base64(bytes);

                        nlohmann::json response = {
                            {"success", true},
                            {"data", encoded_bytecode},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Not A Valid Script Container"},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/getscripthash", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string pointer = json_data.value("pointer", "");

            if (pid.empty() || pointer.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or pointer"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    Instance base_script = client.Get_Pointer_Instance(pointer);
                    if (base_script.ClassName() == "LocalScript" || base_script.ClassName() == "Script") {
                        ScriptClass script = base_script;
                        std::string bytes = script.GetBytecode();
                        std::string encoded_bytecode = base64::to_base64(bytes);

                        nlohmann::json response = {
                            {"success", true},
                            {"data", encoded_bytecode},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else if (base_script.ClassName() == "ModuleScript") {
                        ModuleScriptClass script = base_script;
                        std::string bytes = script.GetBytecode();
                        std::string encoded_bytecode = base64::to_base64(bytes);

                        nlohmann::json response = {
                            {"success", true},
                            {"data", encoded_bytecode},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else {
                        nlohmann::json response = {
                            {"success", false},
                            {"error", "Not A Valid Script Container"},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occured"}
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/queueonteleport", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");
            std::string type = json_data.value("type", "");

            if (pid.empty() || type.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid or type"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    if (type == "get") {
                        json TeleportQueueJ = client.Teleport_Queue;
                        nlohmann::json response = {
                            {"success", true},
                            {"data", TeleportQueueJ}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else if (type == "set") {
                        std::string data = json_data.value("data", "");

                        if (data.empty()) {
                            nlohmann::json response = {
                                {"success", false},
                                {"error", "Missing Required Fields: data"}
                            };
                            res.set_content(response.dump(), "application/json");
                            return;
                        }

                        client.Teleport_Queue.push_back(data);
                        nlohmann::json response = {
                            {"success", true},
                            {"message", "Script Added Successfully To Teleport Queue"},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    else if (type == "clear") {
                        client.Teleport_Queue.clear();
                        nlohmann::json response = {
                            {"success", true},
                            {"message", "Teleport Queue Clear Successful"},
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }
                    nlohmann::json response = {
                        {"success", false},
                        {"error", "Invalid Type"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occurred"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/autoexecute", [Auto_Execute_Directory](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string content;
            for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(Auto_Execute_Directory)) {
                if (!entry.is_regular_file()) {
                    continue;
                }

                std::ifstream file(entry.path());
                if (file.is_open()) {
                    std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    if (Bytecode::Compilable(file_contents).Compiled) {
                        content += "task.spawn(function(...)" + file_contents + "\nend)\n";
                    }
                    file.close();
                }
            }

            nlohmann::json response = {
                {"success", true},
                {"data", content},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    Http_Server.Post("/execute", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");

            if (pid.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    if (client.Execution_Queue.empty()) {
                        nlohmann::json response = {
                            {"success", true},
                            {"data", "Empty"}
                        };
                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    auto source = client.Execution_Queue.front();
                    client.Execution_Queue.pop();

                    nlohmann::json response = {
                        {"success", true},
                        {"data", source}
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occurred"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Console
    Http_Server.Post("/rconsole", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string type = json_data.value("type", "");

            if (type.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: type"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            CreateConsole();

            if (type == "create") {
                nlohmann::json response = {
                    {"success", true},
                    {"message", "Console Allocated Successfully"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else if (type == "destroy") {
                HWND console = GetConsoleWindow();
                if (IsWindowVisible(console)) {
                    ShowWindow(console, SW_HIDE);
                    FreeConsole();
                    Console_Active = false;

                    nlohmann::json response = {
                        {"success", true},
                        {"message", "Console Deallocated Successfully"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                nlohmann::json response = {
                    {"success", true},
                    {"message", "Unable To Deallocated Console"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else if (type == "clear") {
                system("cls");

                nlohmann::json response = {
                    {"success", true},
                    {"message", "Console Cleared Successfully"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else if (type == "title") {
                std::string data = json_data.value("data", "");

                if (data.empty()) {
                    nlohmann::json response = {
                        {"success", false},
                        {"error", "Missing Required Fields: data"}
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
                data = base64::from_base64(data);

                SetConsoleTitleA(data.c_str());

                nlohmann::json response = {
                    {"success", true},
                    {"message", "Console Title Set Successfully"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else if (type == "print") {
                std::string data = json_data.value("data", "");

                if (data.empty()) {
                    nlohmann::json response = {
                        {"success", false},
                        {"error", "Missing Required Fields: data"}
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
                data = base64::from_base64(data);

                std::cout << data << std::endl;

                nlohmann::json response = {
                    {"success", true},
                    {"message", "Printed In Console Successfully"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else if (type == "input") {
                CreateConsole();
                std::string input;
                std::getline(std::cin, input);

                nlohmann::json response = {
                    {"success", true},
                    {"data", base64::to_base64(input)},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }
            else {
                nlohmann::json response = {
                    {"success", false},
                    {"message", "Invalid Type"},
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            nlohmann::json response = {
                {"success", false},
                {"message", "Unknown Error Occured"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Quack
    Http_Server.Post("/printjobs", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            std::string pid = json_data.value("pid", "");

            if (pid.empty()) {
                nlohmann::json response = {
                    {"success", false},
                    {"error", "Missing Required Fields: pid"}
                };
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::shared_lock lock(Clients_Mutex);
            for (auto& client : Clients) {
                if (std::to_string(client.PID) == pid) {
                    client.TaskScheduler.PrintJobs();
                    nlohmann::json response = {
                        {"success", true},
                        {"message", "Jobs Successfully Printed"},
                    };
                    res.set_content(response.dump(), "application/json");
                    return;
                }
            }

            nlohmann::json response = {
                {"success", false},
                {"error", "Unknown Error Occurred"},
            };
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            nlohmann::json response = {
                {"success", false},
                {"error", "Server Error: " + std::string(e.what())}
            };
            res.set_content(response.dump(), "application/json");
        }
    });

    // Http
    Http_Server.set_exception_handler([](const httplib::Request& req, httplib::Response& res, std::exception_ptr ep) {
        std::string errorMessage;
        try {
            std::rethrow_exception(ep);
        }
        catch (std::exception& e) {
            errorMessage = e.what();
        }
        catch (...) {
            errorMessage = "Unknown Exception";
        }
        printf("[SERVER] Exception: %s\n", errorMessage.c_str());
        res.set_content("{\"error\":\"" + errorMessage + "\"}", "application/json");
        res.status = 500;
    });
    Http_Server.listen("localhost", 6767);
};
