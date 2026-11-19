#include <widgets/Checkbox.hpp>
#include <Window.hpp>

namespace litw {

    CheckBox::CheckBox(int x, int y, std::string t, bool& isOn) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->state = &isOn;
    }
    CheckBox::CheckBox(std::string t, bool& isOn) {
        this->x = 0;
        this->y = 0;
        this->text = t;
        this->state = &isOn;
    }
    int CheckBox::GetWidth() const {
        return text.size() + 8;
    }
    int CheckBox::GetHeight() const {
        return 1;
    }
    void CheckBox::Draw(std::ostream& buffer, int px, int py) {
        std::string visual;
        if (focused) {
            visual = *state ? ">  < " : ">  < ";
        }
        else {
            visual = *state ? "     " : "     ";
        }
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        buffer << parent->Palette.Body << visual << text;
    }
    void CheckBox::HandleInput(InputType input) {
        if (input == InputType::Enter) {
            *state = !(*state);
        }
    }

}