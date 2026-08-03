#pragma once
#include "Widget.hpp"
#include <Window.hpp>

class Separator : public Widget {
public:
    int GetWidth() const override {
        return this->parent ? this->parent->width - 2 : 0;
    }
    int GetHeight() const override {
        return 1;
    }
    Separator(int x, int y) {
        this->x = x;
        this->y = y;
        this->isSeparator = true;
        this->focusable = false;
    }
    Separator() {
        this->x = 0;
        this->y = 0;
        this->isSeparator = true;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        if (!this->parent) return;
        buffer << "\033[" << (py + y) << ";" << px << "H";
        buffer << this->parent->Palette.Body;
        int winWidth = this->parent->width;
        if (winWidth > 2) {
            buffer << "├";
            for (int i = 0; i < winWidth - 2; i++) buffer << "─";
            buffer << "┤";
        }
        buffer << "\033[0m";
    }
};