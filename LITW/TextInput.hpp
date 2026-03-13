#pragma once
#include "Widget.hpp"
#include <vector>
class TextInput : public Widget {
public:
    char text[100] = "";
    int textLength = 0;
    TextInput(int x, int y, int z) {
        this->x = x;
        this->y = y;
        z = textLength;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m" << text;
    }
    void HandleInput(InputType input) override {
        bool isWriteing = false;
        if (isWriteing == true) {
            if (input == InputType::Enter) {
                isWriteing = true;
            }
        }
        else {
            int charr = readKey();
            if (charr == '\r') {
                isWriteing == false;
            }
            else if (charr == 8) {
                if (textLength > 0) {
                    textLength--;
                    text[textLength] = '\0';
                }
            }
            else if (textLength < 99) {
                text[textLength] = (char)charr;
                textLength++;
                text[textLength] = '\0';
            }
        }
    }
};