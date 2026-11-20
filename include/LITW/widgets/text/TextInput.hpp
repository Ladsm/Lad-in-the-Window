#pragma once
#include <LITW/Widget.hpp>
#include <LITW/userinput.hpp>
#include <ostream>
#include <string>

namespace litw {

    class TextInput : public Widget {
    public:
        std::string* target;
        bool isWriting = false;
        int maxlength = 10;
        TextInput(int x, int y, int z, std::string* boundString);
        TextInput(int z, std::string* boundString);
        int GetWidth() const override;
        int GetHeight() const override;
        bool WantsRawInput();
        void HandleRawInput();
        void HandleInput(InputType input) override;
        void Draw(std::ostream& buffer, int px, int py) override;
    };

}