#pragma once

#include "Widget.hpp"
#include "userinput.hpp"
#include <ostream>

class TextInput : public Widget {
public:

    char text[100] = "";
    int textLength = 0;
    bool isWriting = false;

    TextInput(int x, int y) {
        this->x = x;
        this->y = y;
    }
    bool WantsRawInput() {
        return isWriting;
    }
    void HandleRawInput() {
        int ch = readKey();
        if (ch == 13) {
            isWriting = false;
            return;
        }
        if (ch == 8 || ch == 127) {
            if (textLength > 0) {
                textLength--;
                text[textLength] = '\0';
            }
            return;
        }
        if (ch >= 32 && ch <= 126 && textLength < 99) {
            text[textLength++] = (char)ch;
            text[textLength] = '\0';
        }
    }
    void HandleInput(InputType input) override {

        if (input == InputType::Enter) {
            isWriting = true;
        }
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";

        if (isWriting)
            buffer << "\033[38;2;0;0;0;48;2;255;255;255m";
        else
            buffer << "\033[38;2;0;0;0;48;2;192;192;192m";

        buffer << text << " ";
    }
};