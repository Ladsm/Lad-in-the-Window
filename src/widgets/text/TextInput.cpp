#include <LITW/widgets/text/TextInput.hpp>
#include <LITW/Window.hpp>

namespace litw {

    TextInput::TextInput(int x, int y, int z, std::string* boundString)
        : target(boundString), maxlength(z)
    {
        this->x = x;
        this->y = y;
    }
    TextInput::TextInput(int z, std::string* boundString)
        : target(boundString), maxlength(z)
    {
        this->x = 0;
        this->y = 0;
    }
    int TextInput::GetWidth() const {
        return maxlength + 1;
    }
    int TextInput::GetHeight() const {
        return 1;
    }
    bool TextInput::WantsRawInput() { return isWriting; }
    void TextInput::HandleRawInput() {
        if (!target) return;
        int ch = readKey();
        if (ch == 13) {
            isWriting = false;
            return;
        }
        if (ch == 8 || ch == 127) {
            if (!target->empty()) {
                target->pop_back();
            }
            return;
        }
        if (ch >= 32 && ch <= 126 && target->length() < maxlength) {
            *target += (char)ch;
        }
    }
    void TextInput::HandleInput(InputType input) {
        if (input == InputType::Enter) isWriting = true;
    }
    void TextInput::Draw(std::ostream& buffer, int px, int py) {
        if (!target) return;
        buffer << "\033[" << (py + y) << ";" << (px + x) << "H";
        if (isWriting) {
            buffer << "\033[38;2;0;0;0;48;2;255;255;255m";
        }
        else {
            buffer << parent->Palette.Body;
        }
        if (focused) {
            buffer << '>' << *target << " \033[0m";
        }
        else {
            buffer << ' ' << *target << " \033[0m";
        }
    }

}