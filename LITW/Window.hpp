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
        auto drawLine = [&](int len) {
            std::string line = "";
            for (int i = 0; i < len; i++) line += "─";
            return line;
            };
        buffer << "\033[" << (y + 1) << ";" << (x + 1) << "H" << headerColor << "┌" << drawLine(innerWidth) << "┐" << reset;
        buffer << "\033[" << (y + 2) << ";" << (x + 1) << "H" << headerColor << "│ ";
        std::string titleText = title;
        int spacing = innerWidth - (int)titleText.length() - iconVisualWidth - 1;
        buffer << titleText << std::string((std::max)(0, spacing), ' ') << "[─] [O] [╳]│" << reset;
        buffer << "\033[" << (y + 3) << ";" << (x + 1) << "H" << headerColor << "├" << drawLine(innerWidth) << "┤" << reset;
        for (int i = 3; i < height - 1; ++i) {
            buffer << "\033[" << (y + i + 1) << ";" << (x + 1) << "H" << silverBody << "│" << std::string(innerWidth, ' ') << "│" << reset;
        }
        buffer << "\033[" << (y + height) << ";" << (x + 1) << "H" << silverBody << "└" << drawLine(innerWidth) << "┘" << reset;
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
            int sw = getConsoleWidth();
            int sh = getConsoleHeight();
            Window* top = nullptr;
            for (int i = (int)windows.size() - 1; i >= 0; --i) {
                if (windows[i]->x < 0) windows[i]->x = 0;
                if (windows[i]->x + windows[i]->width > sw) windows[i]->x = sw - windows[i]->width;
                if (windows[i]->y < 0) windows[i]->y = 0;
                if (windows[i]->y + windows[i]->height > sh - 1) windows[i]->y = (sh - 1) - windows[i]->height;
                windows[i]->focused = false;
                if (!top && windows[i]->visible && !windows[i]->isMinimized) {
                    top = windows[i];
                    top->focused = true;
                }
            }
            std::stringstream frame;
            frame << "\033[H" << wallpaperColor;
            for (int i = 1; i < sh; ++i) {
                frame << "\033[" << i << ";1H\033[K" << std::string(sw, ' ');
            }
            for (auto* w : windows) {
                if (w->visible && !w->isMinimized) w->Draw(frame);
            }
            frame << "\033[" << sh << ";1H\033[97;104m" << std::string(sw, ' ');
            frame << "\033[" << sh << ";1H LITW: ";
            for (int i = 0; i < (int)windows.size(); i++) {
                if (windows[i]->visible) {
                    std::string label = std::to_string(i + 1) + ": " + windows[i]->title + (windows[i]->isMinimized ? " (min)" : "");
                    if (windows[i]->focused) {
                        frame << "\033[97;44m [ " << label << " ] \033[97;104m";
                    }
                    else {
                        frame << "  " << label << "  ";
                    }
                }
            }
            time_t now = time(0);
            struct tm ltm;
            localtime_s(&ltm, &now);
            char timeBuf[16];
            strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &ltm);
            std::string clockStr = " [ " + std::string(timeBuf) + " ] ";
            int clockPos = sw - (int)clockStr.length() + 1;
            frame << "\033[" << sh << ";" << clockPos << "H" << clockStr << "\033[0m";
            std::cout << frame.str() << std::flush;
            InputType input = GetPlayerInput();
            if (input >= InputType::Top1 && input <= InputType::Top9) {
                int targetIdx = static_cast<int>(input) - static_cast<int>(InputType::Top1);
                if (targetIdx < (int)windows.size() && windows[targetIdx]->visible) {
                    windows[targetIdx]->isMinimized = false;
                    Window* selected = windows[targetIdx];
                    windows.erase(windows.begin() + targetIdx);
                    windows.push_back(selected);
                }
            }
            else if (top) {
                if (top->isMoving) {
                    switch (input) {
                    case InputType::MoveUp:    top->y--; break;
                    case InputType::MoveDown:  top->y++; break;
                    case InputType::MoveRight: top->x++; break;
                    case InputType::MoveLeft:  top->x--; break;
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
                    case InputType::Q:      top->isMinimized = true; CycleWindow(); break;
                    default:                top->HandleInput(input); break;
                    }
                }
            }
            else if (input == InputType::Escape) {
                running = false;
            }
        }
        std::cout << "\033[?25h\033[0m\033[2J\033[H" << std::flush;
    }
};