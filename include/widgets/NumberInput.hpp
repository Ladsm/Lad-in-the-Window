#pragma once
#include "Widget.hpp"
#include "userinput.hpp"
#include <ostream>
#include <string>

class NumberInput : public Widget {
public:
    int* target;
    bool isWriting = false;
    int maxlength = 10;
    std::string buffer;
    NumberInput(int x, int y, int z, int* boundInt)
        : target(boundInt), maxlength(z)
    {
        this->x = x;
        this->y = y;
        SyncFromTarget();
    }
    NumberInput(int z, int* boundInt)
        : target(boundInt), maxlength(z)
    {
        this->x = 0;
        this->y = 0;
        SyncFromTarget();
    }
    void SyncFromTarget() {
        if (target) {
            buffer = std::to_string(*target);
        }
    }
    void SyncToTarget() {
        if (!target) return;
        if (buffer.empty() || buffer == "-") {
            *target = 0;
        }
        else {
            try {
                *target = std::stoi(buffer);
            }
            catch (...) {
                *target = 0;
            }
        }
    }

    int GetWidth() const override {
        return maxlength + 1;
    }

    int GetHeight() const override {
        return 1;
    }

    bool WantsRawInput() { return isWriting; }

    void HandleRawInput() {
        if (!target) return;
        int ch = readKey();
        if (ch == 13) {
            isWriting = false;
            return;
        }
        if (ch == 8 || ch == 127) {
            if (!buffer.empty()) {
                buffer.pop_back();
                SyncToTarget();
            }
            return;
        }
        if (ch == '-' && buffer.empty()) {
            buffer += (char)ch;
            return;
        }
        if (ch >= '0' && ch <= '9' && buffer.length() < maxlength) {
            buffer += (char)ch;
            SyncToTarget();
        }
    }

    void HandleInput(InputType input) override {
        if (input == InputType::Enter) {
            isWriting = true;
            SyncFromTarget();
        }
    }

    void Draw(std::ostream& outputBuffer, int px, int py) override {
        if (!target) return;
        outputBuffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        if (isWriting) {
            outputBuffer << "\033[38;2;0;0;0;48;2;255;255;255m";
            outputBuffer << "\033[?25h";
        }
        else {
            outputBuffer << parent->Palette.Body;
            outputBuffer << "\033[?25l";
        }
        if (focused) {
            outputBuffer << '>' << buffer << " \033[0m";
        }
        else {
            outputBuffer << ' ' << buffer << " \033[0m";
        }
    }
};