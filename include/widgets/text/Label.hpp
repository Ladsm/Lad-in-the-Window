#pragma once
#include <Widget.hpp>
#include <Window.hpp>
#include <memory>
class Label : public Widget {
public:
    std::string text;
    Label(int x, int y, std::string t) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->focusable = false;
    }
    Label(std::string t) {
        this->x = 0;
        this->y = 0;
        this->text = t;
        this->focusable = false;
    }
    int GetWidth() const override {
        return (int)text.length();
    }
    int GetHeight() const override {
        return 1;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << parent->Palette.Body << text;
    }
};
class CenteredLabel : public Widget {
public:
    std::string text;
    CenteredLabel(int y, std::string t) : text(t) {
        this->y = y;
        this->focusable = false;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        int centerX = (parent->width / 2) - ((int)text.length() / 2);
        buffer << "\033[" << (py + y) << ";" << (px + (centerX > 0 ? centerX : 1)) << "H";
        buffer << parent->Palette.Body << text;
    }
};
