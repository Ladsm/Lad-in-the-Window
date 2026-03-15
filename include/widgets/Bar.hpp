#pragma once
#include "Widget.hpp"

class Separator : public Widget {
public:
    int width;
    Separator(int x, int y, int w) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->focusable = false;
    }
    void OnResize(int windowWidth, int windowHeight) override {
        this->width = windowWidth;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        if (width > 2) {
            buffer << "├";
            for (int i = 0; i < width - 2; i++) buffer << "─";
            buffer << "┤";
        }
        buffer << "\033[0m";
    }
};