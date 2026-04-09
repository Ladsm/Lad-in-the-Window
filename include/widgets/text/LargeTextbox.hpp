#pragma once
#include "Widget.hpp"
#include <vector>
#include <string>
#include <algorithm>

class TextBox : public Widget {
public:
    std::vector<std::string> text;
    TextBox(int x, int y, const std::vector<std::string>& t) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        for (size_t i = 0; i < text.size(); ++i) {
            buffer << "\033[" << (py + y + static_cast<int>(i)) << ";" << (px + x) << "H";
            buffer << "\033[38;2;0;0;0;48;2;192;192;192m" << text[i] << "\033[0m";
        }
    }
    int GetWidth() const override {
        auto longest_it = std::max_element(text.begin(), text.end(),
            [](const std::string& a, const std::string& b) {
                return a.size() < b.size();
            });
        int returner = std::stoi(*longest_it);
        return returner;
    }
    int GetHeight() const override {
        return text.size();
    }
};