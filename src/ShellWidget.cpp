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
void ShellWidget::Draw(std::ostream& bufferStream, int parentX, int parentY) {
    int drawLines = height - 1;
    int start = 0;
    if ((int)buffer.size() > drawLines)
        start = buffer.size() - drawLines;
    for (int i = 0; i < drawLines; i++) {
        int idx = start + i;
        std::string line = "";
        if (idx < buffer.size())
            line = buffer[idx];
        if (line.length() > width - 2)
            line = line.substr(0, width - 2);
        bufferStream << "\033[" << parentY + y + i << ";" << parentX + x << "H";
        bufferStream << "\033[38;2;0;0;0;48;2;192;192;192m" << line;
    }
    std::string prompt = std::filesystem::current_path().string() + "LITW> " + currentInput;
    if (prompt.length() > width - 2)
        prompt = prompt.substr(prompt.length() - (width - 2));
    bufferStream << "\033[" << parentY + y + height - 1 << ";" << parentX + x << "H";
    bufferStream << "\033[38;2;0;0;0;48;2;192;192;192m" << prompt;
    if (focused)
        bufferStream << "_";
}
void ShellWidget::HandleRawInput() {
    std::string prompt = std::filesystem::current_path().string() + "LITW> " + currentInput;
    int key = readKey();
    if (key == 13) {
        buffer.push_back(prompt + currentInput);
        if (currentInput == "exit") {
            if (auto w = parentWindow.lock())
                w->visible = false;
            currentInput.clear();
            return;
        }
        ExecuteCommand(currentInput);
        currentInput.clear();
        return;
    }
    if (key == 8 || key == 127) {
        if (!currentInput.empty())
            currentInput.pop_back();
        return;
    }
    if (key >= 32 && key <= 126)
        currentInput.push_back((char)key);
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