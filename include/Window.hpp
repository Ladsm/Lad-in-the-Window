#pragma once
#include "getwh.hpp"
#include "userinput.hpp"
#include "Widget.hpp"
#include <vector>
#include <string>
#include <iostream>
#ifdef max
#undef max
#endif
class StartMenuWindow;
class Window {
public:
    int focusedWidget = -1;
    std::vector<Widget*> widgets;
    int x, y, width, height;
    int startWidth, startHeight;
    bool visible = true, focused = false, isMoving = false, isMinimized = false, isResizing = false;
    std::string title;
    int dragOffsetX = 0;
    int dragOffsetY = 0;
    enum ResizeFlag : int {
        RF_None = 0,
        RF_Left = 1 << 0,
        RF_Right = 1 << 1,
        RF_Top = 1 << 2,
        RF_Bottom = 1 << 3
    };
    int resizeFlags = RF_None;
    int resizeStartMouseX = 0;
    int resizeStartMouseY = 0;
    int resizeStartWidth = 0;
    int resizeStartHeight = 0;
    int resizeStartX = 0;
    int resizeStartY = 0;
    Window(std::string t, int w, int h);
    void AddWidget(Widget* w);
    std::string headerColor();
    virtual void Draw(std::ostream& buffer);
    void HandleInput(InputType input);
    bool ContainsPoint(int px, int py) const;
    virtual ~Window();
};
class WindowManager {
    std::vector<Window*> windows;
    int windowCount = 0;
    Window* startMenu = nullptr;
public:
    void Alert(std::string message);
    void SetStartMenu(Window* sm);
    void AddWindow(Window* w);
    void RemoveWindow(Window* w);
    void CycleWindow();
    void Run();
};