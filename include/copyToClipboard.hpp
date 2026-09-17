#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

#ifndef _WIN32
inline bool commandExists(const std::string& cmd) {
    std::string check = "command -v \"" + cmd + "\" > /dev/null 2>&1";
    return (std::system(check.c_str()) == 0);
}
#endif

inline void copyToClipboard(const std::string& text) {
#ifdef _WIN32
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();

    size_t size = text.size() + 1;
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, size);
    if (hGlob) {
        void* lock = GlobalLock(hGlob);
        if (lock) {
            std::memcpy(lock, text.c_str(), size);
            GlobalUnlock(hGlob);
            if (!SetClipboardData(CF_TEXT, hGlob)) {
                GlobalFree(hGlob);
            }
        }
        else {
            GlobalFree(hGlob);
        }
    }
    CloseClipboard();
#else
    std::string command = "";
    if (commandExists("pbcopy")) {
        command = "pbcopy";
    }
    else if (commandExists("wl-copy")) {
        command = "wl-copy";
    }
    else if (commandExists("xclip")) {
        command = "xclip -selection clipboard";
    }
    else if (commandExists("xsel")) {
        command = "xsel --clipboard --input";
    }
    if (!command.empty()) {
        FILE* pipe = popen(command.c_str(), "w");
        if (pipe) {
            std::fwrite(text.c_str(), 1, text.size(), pipe);
            pclose(pipe);
        }
    }
#endif
}
