#pragma once
#include <Widget.hpp>

class CheckBox : public Widget {
public:
    std::string text;
    bool* state;
    CheckBox(int x, int y, std::string t, bool& isOn) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->state = &isOn;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string visual;
        if (focused) {
            visual = *state ? "> [X] < " : "> [ ] < ";
        }
        else {
            visual = *state ? "  [X]   " : "  [ ]   ";
        }
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m" << visual << text;
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Enter) {
            *state = !(*state);
        }
    }
};