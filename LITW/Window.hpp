#pragma once
#include "getwh.hpp"
#include "userinput.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

#ifdef max
#undef max
#endif

class Window {
public:
    int x, y, width, height;
    bool visible = true, focused = false, isMoving = false, isMinimized = false;
    std::string title;

    Window(std::string t, int w, int h) : title(t), width(w), height(h) {
        x = (getConsoleWidth() - w) / 2;
        y = (getConsoleHeight() - h) / 2;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
    }
    virtual void Draw(std::ostream& buffer) {
        if (!visible || isMinimized) return;
        int innerWidth = width - 2;
        const int iconVisualWidth = 11;
        std::string blueHeader = "\033[97;44m";
        std::string silverBody = "\033[38;2;0;0;0;48;2;192;192;192m";
        std::string headerColor = isMoving ? "\033[93;43m" : (focused ? blueHeader : "\033[37;100m");
        std::string reset = "\033[0m";
        buffer << "\033[" << (y + 1) << ";" << (x + 1) << "H" << headerColor << "+" << std::string(innerWidth, '-') << "+" << reset;
        buffer << "\033[" << (y + 2) << ";" << (x + 1) << "H" << headerColor << "| ";
        std::string titleText = title;
        int spacing = innerWidth - (int)titleText.length() - iconVisualWidth - 1;
        buffer << titleText << std::string((std::max)(0, spacing), ' ') << "[─] [O] [╳]|" << reset;
        buffer << "\033[" << (y + 3) << ";" << (x + 1) << "H" << headerColor << "+" << std::string(innerWidth, '-') << "+" << reset;
        for (int i = 3; i < height - 1; ++i) {
            buffer << "\033[" << (y + i + 1) << ";" << (x + 1) << "H" << silverBody << "|" << std::string(innerWidth, ' ') << "|" << reset;
        }
        buffer << "\033[" << (y + height) << ";" << (x + 1) << "H" << silverBody << "+" << std::string(innerWidth, '-') << "+" << reset;
    }
    virtual void HandleInput(InputType input) = 0;
    virtual ~Window() = default;
};

class WindowManager {
    std::vector<Window*> windows;
    int windowCount = 0;
public:
    void AddWindow(Window* w) {
        int offsetIndex = windowCount % 10;
        int offsetX = offsetIndex * 4;
        int offsetY = offsetIndex * 2;
        w->x += offsetX;
        w->y += offsetY;
        if (!windows.empty()) windows.back()->focused = false;
        windows.push_back(w);
        windows.back()->focused = true;
        windowCount++;
    }
    void CycleWindow() {
        if (windows.size() < 2) return;
        Window* top = windows.back();
        top->isMoving = false;
        windows.pop_back();
        windows.insert(windows.begin(), top);
        if (windows.back()->visible && windows.back()->isMinimized) {
            windows.back()->isMinimized = false;
        }
    }
    void Run() {
        std::cout << "\033[2J\033[?25l" << std::flush;
        std::string wallpaperColor = "\033[48;5;30m";
        bool running = true;
        while (running) {
            std::stringstream frame;
            int sw = getConsoleWidth();
            int sh = getConsoleHeight();
            Window* top = nullptr;
            for (int i = (int)windows.size() - 1; i >= 0; --i) {
                windows[i]->focused = false;
                if (!top && windows[i]->visible && !windows[i]->isMinimized) {
                    top = windows[i];
                    top->focused = true;
                }
            }
            frame << "\033[H" << wallpaperColor;
            for (int i = 1; i < sh; ++i) {
                frame << "\033[" << i << ";1H\033[K" << std::string(sw, ' ');
            }
            for (auto* w : windows) {
                if (w->visible && !w->isMinimized) {
                    w->Draw(frame);
                }
            }
            frame << "\033[" << sh << ";1H\033[97;104m LITW: ";
            int usedWidth = 7;
            for (int i = 0; i < (int)windows.size(); i++) {
                Window* w = windows[i];
                if (w->visible) {
                    std::string label = std::to_string(i + 1) + ": " + w->title + (w->isMinimized ? " (min)" : "");
                    std::string btn = w->focused ? " [ " + label + " ] " : "  " + label + "  ";
                    usedWidth += (int)btn.length();
                    if (w->focused) frame << "\033[97;44m" << btn << "\033[97;104m";
                    else frame << btn;
                }
            }
            time_t now = time(0);
            struct tm ltm;
            localtime_s(&ltm, &now);
            char timeBuf[16];
            strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &ltm);
            std::string clockStr = " [ " + std::string(timeBuf) + " ] ";
            int clockPadding = sw - usedWidth - (int)clockStr.length();
            if (clockPadding > 0) frame << std::string(clockPadding, ' ');
            frame << clockStr << "\033[0m";
            std::cout << frame.str() << std::flush;
            InputType input = GetPlayerInput();
            if (input >= InputType::Top1 && input <= InputType::Top9) {
                int targetIdx = static_cast<int>(input) - static_cast<int>(InputType::Top1);
                if (targetIdx < (int)windows.size()) {
                    Window* selected = windows[targetIdx];
                    if (selected->visible) {
                        selected->isMinimized = false;
                        windows.erase(windows.begin() + targetIdx);
                        windows.push_back(selected);
                        continue;
                    }
                }
            }

            if (top) {
                if (top->isMoving) {
                    switch (input) {
                    case InputType::MoveUp:    if (top->y > 0) top->y--; break;
                    case InputType::MoveDown:  if (top->y < sh - top->height - 1) top->y++; break;
                    case InputType::MoveRight: if (top->x > 0) top->x++; break;
                    case InputType::MoveLeft:  if ((top->x + top->width) < sw) top->x--; break;
                    case InputType::E: case InputType::Enter: top->isMoving = false; break;
                    case InputType::Escape:    top->isMoving = false; CycleWindow(); break;
                    default: break;
                    }
                }
                else {
                    switch (input) {
                    case InputType::Escape: CycleWindow(); break;
                    case InputType::X:      top->visible = false; break;
                    case InputType::E:      top->isMoving = true; break;
                    case InputType::Q:      top->isMinimized = true; top->focused = false; CycleWindow(); break;
                    default:                top->HandleInput(input); break;
                    }
                }
            }
            else {
                if (input == InputType::Escape) running = false;
            }
        }
        std::cout << "\033[?25h\033[0m\033[2J\033[H" << std::flush;
    }
};