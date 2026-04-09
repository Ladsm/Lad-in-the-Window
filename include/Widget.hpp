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
    bool focusable = true;
    bool IsContainer = false;
    bool isSeparator = false;
    virtual int GetWidth() const { return 10; }
    virtual int GetHeight() const { return 1; }
    virtual void Draw(std::ostream& buffer, int parentX, int parentY) = 0;
    virtual void HandleInput(InputType input) {}
    virtual ~Widget() = default;
    virtual bool WantsRawInput() { return false; }
    virtual void HandleRawInput() {}
    virtual Widget* GetActiveWidget() { return this; }
    virtual void OnResize(int windowWidth, int windowHeight) {}
};