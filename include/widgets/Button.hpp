#pragma once
#include "Widget.hpp"
#include <functional>
class Button : public Widget {
public:
    std::string label;
    std::function<void()> onClick;

    Button(int x, int y, std::string l, std::function<void()> cb) {
        this->x = x;
        this->y = y;
        label = l;
        onClick = cb;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string body = "\033[38;2;0;0;0;48;2;192;192;192m";
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << body;
        if (focused)
            buffer << ">[" << label << "]<";
        else
            buffer << " [" << label << "] ";
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Enter && onClick) onClick();
    }
};