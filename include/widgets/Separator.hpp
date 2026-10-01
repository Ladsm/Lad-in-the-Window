#pragma once
#include "Widget.hpp"
#include <Window.hpp>

class HorizontalSeparator : public Widget {
public:
    int GetWidth() const override {
        return this->parent ? this->parent->width - 2 : 0;
    }
    int GetHeight() const override {
        return 1;
    }
    HorizontalSeparator(int x, int y) {
        this->x = x;
        this->y = y;
        this->isSeparator = true;
        this->focusable = false;
    }
    HorizontalSeparator() {
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

class VerticalSeparator : public Widget {
public:
    int GetWidth() const override {
        return 1;
    }

    int GetHeight() const override {
        return parent ? parent->height - 2 : 0;
    }

    VerticalSeparator(int x) {
        this->x = x;
        this->y = 0;
        this->isSeparator = true;
        this->focusable = false;
    }

    void Draw(std::ostream& buffer, int px, int py) override {
        if (!this->parent) return;

        int drawX = px + this->x;
        int drawY = py + this->y;
        int h = GetHeight();

        if (h <= 0) return;

        buffer << this->parent->headerColor();
        buffer << "\033[" << drawY << ";" << drawX << "H┬";
        buffer << this->parent->Palette.Body;
        for (int i = 1; i < h - 1; i++) {
            buffer << "\033[" << (drawY + i) << ";" << drawX << "H│";
        }
        if (h > 1) {
            buffer << "\033[" << (drawY + h - 1) << ";" << drawX << "H┴";
        }

        buffer << "\033[0m";
    }
};