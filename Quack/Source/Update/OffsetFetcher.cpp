#include "Update/OffsetFetcher.hpp"

#include <Windows.h>
#include <winhttp.h>
#include <vector>
#include <string>
#include <algorithm>

namespace {
    bool ParseUrl(const std::wstring& url, std::wstring& host, std::wstring& path, bool& secure) {
        auto schemePos = url.find(L"://");
        if (schemePos == std::wstring::npos) return false;
        auto scheme = url.substr(0, schemePos);
        secure = (scheme == L"https");
        auto rest = url.substr(schemePos + 3);
        auto slashPos = rest.find(L'/');
        if (slashPos == std::wstring::npos) {
            host = rest;
            path = L"/";
        } else {
            host = rest.substr(0, slashPos);
            path = rest.substr(slashPos);
        }
        return !host.empty();
    }
}

bool OffsetFetcher::DownloadText(const std::wstring& url, std::string& output) {
    std::wstring host;
    std::wstring path;
    bool secure = false;
    if (!ParseUrl(url, host, path, secure)) {
        return false;
    }

    HINTERNET session = WinHttpOpen(L"Quack Offset Fetcher/1.0", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) {
        return false;
    }

    HINTERNET connect = WinHttpConnect(session, host.c_str(), secure ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!connect) {
        WinHttpCloseHandle(session);
        return false;
    }

    HINTERNET request = WinHttpOpenRequest(connect, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, secure ? WINHTTP_FLAG_SECURE : 0);
    if (!request) {
        WinHttpCloseHandle(connect);
        WinHttpCloseHandle(session);
        return false;
    }

    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) || !WinHttpReceiveResponse(request, nullptr)) {
        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connect);
        WinHttpCloseHandle(session);
        return false;
    }

    DWORD available = 0;
    while (WinHttpQueryDataAvailable(request, &available) && available > 0) {
        std::vector<char> buffer(available);
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), available, &read) || read == 0) {
            break;
        }
        output.append(buffer.data(), read);
    }

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return !output.empty();
}
