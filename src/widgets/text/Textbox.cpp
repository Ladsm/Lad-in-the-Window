#include <LITW/widgets/text/Textbox.hpp>
#include <LITW/Window.hpp>
#include <algorithm>

namespace litw {

    TextBox::TextBox(int x, int y, const std::vector<std::string>& t) {
        this->x = x;
        this->y = y;
        this->text = t;
        this->focusable = false;
    }
    TextBox::TextBox(const std::vector<std::string>& t) {
        this->x = 0;
        this->y = 0;
        this->text = t;
        this->focusable = false;
    }
    void TextBox::Draw(std::ostream& buffer, int px, int py) {
        for (size_t i = 0; i < text.size(); ++i) {
            buffer << "\033[" << (py + y + static_cast<int>(i)) << ";" << (px + x) << "H";
            buffer << parent->Palette.Body << text[i] << "\033[0m";
        }
    }
    int TextBox::GetWidth() const {
        if (text.empty()) {
            return 0;
        }
        auto longest_it = std::max_element(text.begin(), text.end(),
            [](const std::string& a, const std::string& b) {
                return a.size() < b.size();
            });
        return static_cast<int>(longest_it->size());
    }
    int TextBox::GetHeight() const {
        return text.size();
    }

}