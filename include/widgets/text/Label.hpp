#pragma once
#include <Widget.hpp>

namespace litw {

    class Label : public Widget {
    public:
        std::string text;
        Label(int x, int y, std::string t);
        Label(std::string t);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
    };

}