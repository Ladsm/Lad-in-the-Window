#pragma once
#include "Widget.hpp"
class Label : public Widget {
public:
    std::string text;
    Label(int x, int y, std::string t) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m" << text;
    }
};