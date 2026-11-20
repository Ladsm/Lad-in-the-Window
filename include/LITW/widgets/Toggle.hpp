#pragma once
#include <LITW/Widget.hpp>

namespace litw {

    class Toggle : public Widget {
    public:
        std::string label;
        bool& value;
        Toggle(int x, int y, std::string l, bool& val);
        Toggle(std::string l, bool& val);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
        void HandleInput(InputType input) override;
    };

}