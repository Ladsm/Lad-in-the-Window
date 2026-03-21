#pragma once
#include "Widget.hpp"
#include <Window.hpp>

class Separator : public Widget {
public:
    Window* parent;
    int GetWidth() const override {
        return parent->width;
    }
    int GetHeight() const override {
        return 1;
    }
    Separator(int x, int y, Window* p) : parent(p) {
        this->x = x;
        this->y = y;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        int currentWidth = parent->width;
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        if (currentWidth > 2) {
            buffer << "├";
            for (int i = 0; i < currentWidth - 2; i++) {
                buffer << "─";
            }
            buffer << "┤";
        }
        buffer << "\033[0m";
    }
};