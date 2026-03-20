#include <widgets/ShellWidget.hpp>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <mutex>
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif
ShellWidget::ShellWidget(int px, int py, std::weak_ptr<Window> parent) {
    x = px;
    y = py;
    parentWindow = parent;
}
void ShellWidget::HandleRawInput() {
    auto parent = parentWindow.lock();
    if (!parent) return;
    int currentWidth = parent->width - 4;
    int key = readKey();
    if (key == 13) {
        if (currentInput == "clear" || currentInput == "cls") {
            buffer.clear();
        }
        if (currentInput == "exit") {
            parent->visible = false;
        }
        if (currentInput == "") {
            isWriting = false;
        }
        else if (!currentInput.empty()) {
            buffer.push_back("LITW " + std::filesystem::current_path().string() + "> " + currentInput);
            ExecuteCommand(currentInput);
        }
        currentInput.clear();
        return;
    }
    if (key == 9) {
        std::string toMatch = currentInput;
        size_t lastSpace = toMatch.find_last_of(" ");
        std::string prefix = (lastSpace == std::string::npos) ? "" : toMatch.substr(0, lastSpace + 1);
        std::string search = (lastSpace == std::string::npos) ? toMatch : toMatch.substr(lastSpace + 1);
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
            std::string name = entry.path().filename().string();
            if (name.rfind(search, 0) == 0) {
                currentInput = prefix + name;
                break;
            }
        }
        return;
    }
    if (key == 8 || key == 127) {
        if (!currentInput.empty()) currentInput.pop_back();
        return;
    }
    if (key >= 32 && key <= 126 && currentInput.length() < (size_t)(currentWidth - 5)) {
        currentInput += (char)key;
    }
}

void ShellWidget::Draw(std::ostream& bufferStream, int parentX, int parentY) {
    auto parent = parentWindow.lock();
    if (!parent) return;
    std::lock_guard<std::mutex> lock(bufferMutex);
    int currentWidth = parent->width - 2;
    int currentHeight = parent->height - 4;
    int drawLines = currentHeight - 1;
    int start = (buffer.size() > (size_t)drawLines) ? (int)buffer.size() - drawLines : 0;
    for (int i = 0; i < drawLines; i++) {
        int idx = start + i;
        std::string line = (idx < (int)buffer.size()) ? buffer[idx] : "";
        if (line.length() > (size_t)currentWidth - 2)
            line = line.substr(0, currentWidth - 2);
        bufferStream << "\033[" << parentY + y + i << ";" << parentX + x << "H"
            << "\033[38;2;0;0;0;48;2;192;192;192m" << line << "\033[0m";
    }
    std::string prompt = "LITW " + std::filesystem::current_path().string() + "> " + currentInput;
    bufferStream << "\033[" << parentY + y + currentHeight - 1 << ";" << parentX + x << "H";
    bufferStream << "\033[38;2;0;0;0;48;2;192;192;192m\033[?25l";
    if (focused && !isWriting) {
        bufferStream << " " << prompt << " \033[0m";
    }
    else {
        bufferStream << ">" << prompt << " \033[0m";
    }
}

void ShellWidget::ExecuteCommand(const std::string& cmd) {
    if (cmd.rfind("cd ", 0) == 0) {
        std::string path = cmd.substr(3);
        try {
            std::filesystem::current_path(path);
            std::lock_guard<std::mutex> lock(bufferMutex);
            buffer.push_back("Changed directory to: " + std::filesystem::current_path().string());
        }
        catch (...) {
            std::lock_guard<std::mutex> lock(bufferMutex);
            buffer.push_back("cd: directory not found");
        }
        return;
    }
    std::thread([this, cmd]() {
        FILE* pipe = POPEN(cmd.c_str(), "r");
        if (!pipe) {
            std::lock_guard<std::mutex> lock(bufferMutex);
            buffer.push_back("Failed to run command.");
            return;
        }
        char line[256];
        while (fgets(line, sizeof(line), pipe)) {
            std::string s = line;
            if (!s.empty() && s.back() == '\n') s.pop_back();
            std::lock_guard<std::mutex> lock(bufferMutex);
            buffer.push_back(s);
            if (buffer.size() > 500) buffer.erase(buffer.begin());
        }
        PCLOSE(pipe);
        }).detach();
}