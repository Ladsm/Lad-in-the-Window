#include <widgets/text/Label.hpp>
#include <Window.hpp>

namespace litw {

    Label::Label(int x, int y, std::string t) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->focusable = false;
    }
    Label::Label(std::string t) {
        this->x = 0;
        this->y = 0;
        this->text = t;
        this->focusable = false;
    }
    int Label::GetWidth() const {
        return (int)text.length();
    }
    int Label::GetHeight() const {
        return 1;
    }
    void Label::Draw(std::ostream& buffer, int px, int py) {
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << parent->Palette.Body << text;
    }

}