#include <widgets/ShellWidget.hpp>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif
ShellWidget::ShellWidget(int px, int py, int w, int h, std::weak_ptr<Window> parent) {
    x = px;
    y = py;
    width = w;
    height = h;
    parentWindow = parent;
}
void ShellWidget::HandleRawInput() {
    int key = readKey();
    if (key == 13) {
        if (currentInput == "exit") {
            if (auto w = parentWindow.lock()) w->visible = false;
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
    if (key == 8 || key == 127) {
        if (!currentInput.empty()) currentInput.pop_back();
        return;
    }
    if (key >= 32 && key <= 126 && currentInput.length() < (size_t)(width - 5)) {
        currentInput += (char)key;
    }
}
void ShellWidget::Draw(std::ostream& bufferStream, int parentX, int parentY) {
    int drawLines = height - 1;
    int start = (buffer.size() > (size_t)drawLines) ? buffer.size() - drawLines : 0;
    for (int i = 0; i < drawLines; i++) {
        int idx = start + i;
        std::string line = (idx < (int)buffer.size()) ? buffer[idx] : "";
        if (line.length() > (size_t)width - 2) line = line.substr(0, width - 2);
        bufferStream << "\033[" << parentY + y + i << ";" << parentX + x << "H" << "\033[38;2;0;0;0;48;2;192;192;192m" << line << "\033[0m";
    }
    std::string prompt = "LITW " + std::filesystem::current_path().string() + "> " + currentInput;
    bufferStream << "\033[" << parentY + y + height - 1 << ";" << parentX + x << "H";
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
            buffer.push_back("Changed directory to: " + std::filesystem::current_path().string());
        }
        catch (...) {
            buffer.push_back("cd: directory not found");
        }
        return;
    }
    FILE* pipe = POPEN(cmd.c_str(), "r");
    if (!pipe) {
        buffer.push_back("Failed to run command.");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), pipe)) {
        std::string s = line;
        if (!s.empty() && s.back() == '\n')
            s.pop_back();
        buffer.push_back(s);
    }
    PCLOSE(pipe);
}