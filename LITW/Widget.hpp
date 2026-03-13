#pragma once
#include <iostream>
#include <string>
#include "userinput.hpp"
class Widget {
public:
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    bool focused = false;
    virtual void Draw(std::ostream& buffer, int parentX, int parentY) = 0;
    virtual void HandleInput(InputType input) {}
    virtual ~Widget() = default;
    virtual bool WantsRawInput() { return false; }
    virtual void HandleRawInput() {}
};