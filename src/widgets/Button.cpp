#include <widgets/Button.hpp>
#include <Window.hpp>

namespace litw {

    Button::Button(int x, int y, std::string l, std::function<void()> cb, bool brack) {
        this->x = x;
        this->y = y;
        label = l;
        onClick = cb;
        brackets = brack;
        this->focusable = true;
    }
    Button::Button(std::string l, std::function<void()> cb) {
        this->x = 0;
        this->y = 0;
        label = l;
        onClick = cb;
        this->focusable = true;
    }
    Button::Button(std::string l, std::function<void()> cb, bool brack) {
        this->x = 0;
        this->y = 0;
        label = l;
        onClick = cb;
        brackets = brack;
        this->focusable = true;
    }
    int Button::GetWidth() const {
        if (!brackets) return (int)label.length() + 4;
        else return (int)label.length() + 2;
    }
    int Button::GetHeight() const {
        return 1;
    }
    void Button::Draw(std::ostream& buffer, int px, int py) {
        std::string body = parent->Palette.Body;
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << body;
        if (brackets) {
            if (focused) {
                buffer << ">[" << label << "]<";
            }
            else {
                buffer << " [" << label << "] ";
            }
        }
        else {
            if (focused) {
                buffer << ">" << label << "<";
            }
            else {
                buffer << " " << label << " ";
            }
        }
    }
    void Button::HandleInput(InputType input) {
        if (input == InputType::Enter && onClick)
            onClick();
    }

}