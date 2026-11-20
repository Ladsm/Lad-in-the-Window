#pragma once
#include <LITW/Widget.hpp>
#include <vector>

namespace litw {

    class ScrollableTextBox : public Widget {
    public:
        int scroll = 0;
        int amountOfLinesToDraw = 10;
        int width = 30;
        std::vector<std::string> text;
        ScrollableTextBox(int x, int y, int h, int w, const std::vector<std::string>& t);
        ScrollableTextBox(int h, int w, const std::vector<std::string>& t);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
        void HandleInput(InputType input) override;
    };

}