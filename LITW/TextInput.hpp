#pragma once
#include "Widget.hpp"
#include "userinput.hpp"
#include <ostream>
#include <string>

class TextInput : public Widget {
public:
    std::string& target;
    bool isWriting = false;
    int maxlength = 10;
    TextInput(int x, int y, int z, std::string& boundString)
        : target(boundString), maxlength(z)
    {
        this->x = x;
        this->y = y;
    }
    bool WantsRawInput() { return isWriting; }
    void HandleRawInput() {
        int ch = readKey();
        if (ch == 13) {
            isWriting = false;
            return;
        }
        if (ch == 8 || ch == 127) {
            if (!target.empty()) target.pop_back();
            return;
        }
        if (ch >= 32 && ch <= 126 && target.length() < maxlength) {
            target += (char)ch;
        }
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Enter) isWriting = true;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        if (isWriting) {
            buffer << "\033[38;2;0;0;0;48;2;255;255;255m";
            buffer << "\033[?25h";
        }
        else { buffer << "\033[38;2;0;0;0;48;2;192;192;192m"; buffer << "\033[?25l"; }
        buffer << target << " \033[0m";
    }
};