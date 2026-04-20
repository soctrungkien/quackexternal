#pragma once

#include <string>

namespace OffsetFetcher {
    bool DownloadText(const std::wstring& url, std::string& output);
}
