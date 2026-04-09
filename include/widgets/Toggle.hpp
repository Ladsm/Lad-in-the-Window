#pragma once
#include "Widget.hpp"

class Toggle : public Widget {
public:
    std::string label;
    bool& value;
    Toggle(int x, int y, std::string l, bool& val) : value(val) {
        this->x = x;
        this->y = y;
        label = l;
        this->focusable = true;
    }
    Toggle(std::string l, bool& val) : value(val) {
        this->x = 0;
        this->y = 0;
        label = l;
        this->focusable = true;
    }
    int GetWidth() const override { return (int)label.length() + 8; }
    int GetHeight() const override { return 1; }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string body = "\033[38;2;0;0;0;48;2;192;192;192m";
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        if (focused) buffer << body << ">";
        else buffer << body << " ";
        buffer << label << ": [" << (value ? "ON " : "OFF") << "]";
        if (focused) buffer << "<";
        buffer << body;
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Enter) {
            value = !value;
        }
    }
};
