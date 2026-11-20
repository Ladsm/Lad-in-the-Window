#pragma once
#include <LITW/Widget.hpp>

namespace litw {

    class CheckBox : public Widget {
    public:
        std::string text;
        bool* state;
        CheckBox(int x, int y, std::string t, bool& isOn);
        CheckBox(std::string t, bool& isOn);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
        void HandleInput(InputType input) override;
    };

}