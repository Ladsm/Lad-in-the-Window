#include <widgets/Toggle.hpp>
#include <Window.hpp>

namespace litw {

    Toggle::Toggle(int x, int y, std::string l, bool& val) : value(val) {
        this->x = x;
        this->y = y;
        label = l;
        this->focusable = true;
    }
    Toggle::Toggle(std::string l, bool& val) : value(val) {
        this->x = 0;
        this->y = 0;
        label = l;
        this->focusable = true;
    }
    int Toggle::GetWidth() const { return (int)label.length() + 8; }
    int Toggle::GetHeight() const { return 1; }
    void Toggle::Draw(std::ostream& buffer, int px, int py) {
        std::string body = parent->Palette.Body;
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << body << label << ":";
        if (focused) buffer << body << ">";
        else buffer << body << " ";
        buffer << body << "[" << (value ? "ON " : "OFF") << "]";
        if (focused) buffer << "<";
        buffer << body;
    }
    void Toggle::HandleInput(InputType input) {
            if (input == InputType::Enter) {
                value = !value;
            }
    }

}