#pragma once
#include <LITW/Widget.hpp>

namespace litw {

    class HorizontalSeparator : public Widget {
    public:
        int GetWidth() const override;
        int GetHeight() const override;
        HorizontalSeparator(int x, int y);
        HorizontalSeparator();
        void Draw(std::ostream& buffer, int px, int py) override;
    };

    class VerticalSeparator : public Widget {
    public:
        int GetWidth() const override;
        int GetHeight() const override;
        VerticalSeparator(int x);
        void Draw(std::ostream& buffer, int px, int py) override;
    };

}