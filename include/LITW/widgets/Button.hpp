#pragma once
#include <LITW/Widget.hpp>
#include <functional>

namespace litw {

    class Button : public Widget {
    public:
        std::string label;
        std::function<void()> onClick;
        bool brackets = true;
        Button(int x, int y, std::string l, std::function<void()> cb, bool brack);
        Button(std::string l, std::function<void()> cb);
        Button(std::string l, std::function<void()> cb, bool brack);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
        void HandleInput(InputType input) override;
    };

}