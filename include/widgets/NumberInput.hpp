#pragma once
#include "Widget.hpp"
#include "userinput.hpp"
#include <ostream>
#include <string>
#include <sstream>
#include <type_traits>

template <typename T>
class NumberInput : public Widget {
public:
    T* target;
    bool isWriting = false;
    int maxlength = 10;
    std::string buffer;

    NumberInput(int x, int y, int z, T* boundValue)
        : target(boundValue), maxlength(z)
    {
        this->x = x;
        this->y = y;
        SyncFromTarget();
    }

    NumberInput(int z, T* boundValue)
        : target(boundValue), maxlength(z)
    {
        this->x = 0;
        this->y = 0;
        SyncFromTarget();
    }

    void SyncFromTarget() {
        if (target) {
            std::ostringstream oss;
            oss << *target;
            buffer = oss.str();
        }
    }

    void SyncToTarget() {
        if (!target) return;
        if (buffer.empty() || buffer == "-") {
            *target = static_cast<T>(0);
        }
        else {
            try {
                std::istringstream iss(buffer);
                T val;
                if (iss >> val) {
                    *target = val;
                }
                else {
                    *target = static_cast<T>(0);
                }
            }
            catch (...) {
                *target = static_cast<T>(0);
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
            if (!buffer.empty() && buffer.back() == '.') {
                buffer.pop_back();
                SyncToTarget();
            }
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

        if constexpr (std::is_floating_point_v<T>) {
            if (ch == '.' && buffer.find('.') == std::string::npos) {
                if (buffer.empty()) {
                    buffer += '0';
                }
                else if (buffer.length() == 1 && buffer[0] == '-') {
                    buffer += '0';
                }
                buffer += (char)ch;
                return;
            }
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
        }
        else {
            outputBuffer << parent->Palette.Body;
        }
        if (focused) {
            outputBuffer << '>' << buffer << " \033[0m";
        }
        else {
            outputBuffer << ' ' << buffer << " \033[0m";
        }
    }
};