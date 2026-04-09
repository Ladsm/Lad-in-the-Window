#pragma once
#include "Widget.hpp"
#include <Window.hpp>
class Separator : public Widget {
public:
    Window* parent;
    int GetWidth() const override {
        return parent->width - 2;
    }
    int GetHeight() const override {
        return 1;
    }
    Separator(int x, int y, Window* p) : parent(p) {
        this->x = x;
        this->y = y;
        this->isSeparator = true;
        this->focusable = false;
    }
    Separator(Window* p) : parent(p) {
        this->x = 0;
        this->y = 0;
        this->isSeparator = true;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << px << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        int winWidth = parent->width;
        if (winWidth > 2) {
            buffer << "├";
            for (int i = 0; i < winWidth - 2; i++) buffer << "─";
            buffer << "┤";
        }
        buffer << "\033[0m";
    }
};