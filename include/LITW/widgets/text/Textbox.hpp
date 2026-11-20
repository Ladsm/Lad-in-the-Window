#pragma once
#include <LITW/Widget.hpp>
#include <vector>
#include <string>

namespace litw {

    class TextBox : public Widget {
    public:
        std::vector<std::string> text;
        TextBox(int x, int y, const std::vector<std::string>& t);
        TextBox(const std::vector<std::string>& t);
        void Draw(std::ostream& buffer, int px, int py);
        int GetWidth() const override;
        int GetHeight() const override;
    };

}