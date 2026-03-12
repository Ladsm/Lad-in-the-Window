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

class StartMenuWindow;

class Window {
public:
    int x, y, width, height;
    bool visible = true, focused = false, isMoving = false, isMinimized = false, isResizing = false;
    std::string title;
    Window(std::string t, int w, int h) : title(t), width(w), height(h) {
        x = (getConsoleWidth() - w) / 2;
        y = (getConsoleHeight() - h) / 2;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
    }
        std::string headerColor(){
            if (isResizing == true) {
                return "\033[95;45m";
            }
            else if (isMoving == true) {
                return "\033[93;43m";
            }
            else {
                if (focused == true) {
                    return "\033[97;44m";
                }
                else {
                    return "\033[37;100m";
                }
            }
        }
    virtual void Draw(std::ostream& buffer) {
        if (!visible || isMinimized) return;
        int innerWidth = width - 2;
        const int iconVisualWidth = 11;
        std::string blueHeader = "\033[97;44m";
        std::string silverBody = "\033[38;2;0;0;0;48;2;192;192;192m";
        std::string resetStyle = "\033[0m";
        auto drawLine = [&](int len) {
            std::string line;
            for (int i = 0; i < len; i++) line += "─";
            return line;
            };
        buffer << "\033[" << (y + 1) << ";" << (x + 1) << "H" << headerColor() << "┌" << drawLine(innerWidth) << "┐" << resetStyle;
        buffer << "\033[" << (y + 2) << ";" << (x + 1) << "H" << headerColor() << "│ ";
        std::string titleText = title;
        int spacing = innerWidth - (int)titleText.length() - iconVisualWidth - 1;
        buffer << titleText << std::string((std::max)(0, spacing), ' ') << "[─] [O] [╳]│" << resetStyle;
        buffer << "\033[" << (y + 3) << ";" << (x + 1) << "H" << headerColor() << "├" << drawLine(innerWidth) << "┤" << resetStyle;

        for (int i = 3; i < height - 1; ++i) {
            buffer << "\033[" << (y + i + 1) << ";" << (x + 1) << "H" << silverBody << "│" << std::string(innerWidth, ' ') << "│" << resetStyle;
        }
        buffer << "\033[" << (y + height) << ";" << (x + 1) << "H" << silverBody << "└" << drawLine(innerWidth) << "┘" << resetStyle;
    }
    virtual void HandleInput(InputType input) = 0;
    virtual ~Window() = default;
};

class WindowManager {
    std::vector<Window*> windows;
    int windowCount = 0;
    Window* startMenu = nullptr;
public:
    void SetStartMenu(Window* sm) {
        startMenu = sm;
    }
    void AddWindow(Window* w) {
        int offsetIndex = windows.size() % 10;
        int offsetX = offsetIndex * 4;
        int offsetY = offsetIndex * 2;
        w->x += offsetX;
        w->y += offsetY;
        if (!windows.empty())
            windows.back()->focused = false;
        windows.push_back(w);
        windows.back()->focused = true;
        windowCount++;
    }
    void RemoveWindow(Window* w) {
        auto it = std::find(windows.begin(), windows.end(), w);
        if (it != windows.end()) {
            windows.erase(it);
            windowCount--;
        }
    }
    void CycleWindow() {
        if (windows.size() < 2) return;
        Window* top = windows.back();
        top->isMoving = false;
        windows.pop_back();
        windows.insert(windows.begin(), top);
        if (windows.back()->visible && windows.back()->isMinimized)
            windows.back()->isMinimized = false;
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
                // Keep position inside screen
                if (windows[i]->x < 0) windows[i]->x = 0;
                if (windows[i]->x + windows[i]->width > sw)
                    windows[i]->x = sw - windows[i]->width;
                if (windows[i]->y < 0) windows[i]->y = 0;
                if (windows[i]->y + windows[i]->height > sh - 1)
                    windows[i]->y = (sh - 1) - windows[i]->height;

                // Prevent windows from being larger than the console
                if (windows[i]->width > sw) windows[i]->width = sw;
                if (windows[i]->height > (sh - 1)) windows[i]->height = sh - 1;

                windows[i]->focused = false;
                if (!top && windows[i]->visible && !windows[i]->isMinimized) {
                    top = windows[i];
                    top->focused = true;
                }
            }
            std::stringstream frame;
            frame << "\033[H" << wallpaperColor;
            for (int i = 1; i < sh; ++i) {
                frame << "\033[" << i << ";1H\033[K"
                    << std::string(sw, ' ');
            }
            for (auto* w : windows) {
                if (w->visible && !w->isMinimized)
                    w->Draw(frame);
            }
            frame << "\033[" << sh << ";1H\033[97;104m" << std::string(sw, ' ');
            frame << "\033[" << sh << ";1H Start(C)  LITW: ";
            for (int i = 0; i < (int)windows.size(); i++) {
                if (windows[i]->visible) {
                    std::string label =
                        std::to_string(i + 1) + ": " +
                        windows[i]->title +
                        (windows[i]->isMinimized ? " (min)" : "");

                    if (windows[i]->focused)
                        frame << "\033[97;44m [ " << label << " ] \033[97;104m";
                    else
                        frame << "  " << label << "  ";
                }
            }
            time_t now = time(0);
            struct tm ltm;
#ifdef _WIN32
            localtime_s(&ltm, &now);
#else
            ltm = *localtime(&now);
#endif
            char timeBuf[16];
            strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &ltm);
            std::string clockStr = " [ " + std::string(timeBuf) + " ] ";
            int clockPos = sw - (int)clockStr.length() + 1;
            frame << "\033[" << sh << ";" << clockPos << "H" << clockStr << "\033[0m";
            std::cout << frame.str() << std::flush;
            InputType input = GetPlayerInput();
            if (input == InputType::C && startMenu) {
                startMenu->visible = !startMenu->visible;
                if (startMenu->visible) {
                    if (std::find(windows.begin(), windows.end(), startMenu) == windows.end())
                        windows.push_back(startMenu);
                }
                else {
                    windows.erase(std::remove(windows.begin(), windows.end(), startMenu), windows.end());
                }
                continue;
            }
            if (input >= InputType::Top1 && input <= InputType::Top9) {
                int targetIdx =
                    static_cast<int>(input) -
                    static_cast<int>(InputType::Top1);
                if (targetIdx < (int)windows.size() &&
                    windows[targetIdx]->visible) {
                    windows[targetIdx]->isMinimized = false;
                    Window* selected = windows[targetIdx];
                    windows.erase(windows.begin() + targetIdx);
                    windows.push_back(selected);
                }
            }
            else if (top) {
                if (top->isMoving == true) {
                    switch (input) {
                    case InputType::MoveUp:    top->y--; break;
                    case InputType::MoveDown:  top->y++; break;
                    case InputType::MoveRight: top->x++; break;
                    case InputType::MoveLeft:  top->x--; break;
                    case InputType::R:
                        if (top != startMenu) {
                            top->isMoving = false;
                            top->isResizing = true;
                        }
                        break;
                    case InputType::E:
                    case InputType::Enter:
                        top->isMoving = false;
                        break;
                    case InputType::Escape:
                        top->isMoving = false;
                        CycleWindow();
                        break;
                    default:
                        break;
                    }
                }
                else if (top->isResizing == true) {
                    int minHeight = 3;
                    int minWidth = 4;
                    int maxHeight = sh - top->y - 1;
                    int maxWidth = sw - top->x;
                    if (maxHeight < minHeight) maxHeight = minHeight;
                    if (maxWidth < minWidth) maxWidth = minWidth;
                    switch (input) {
                    case InputType::MoveUp:
                        if (top->height > minHeight) top->height--;
                        break;
                    case InputType::MoveDown:
                        if (top->height < maxHeight) top->height++;
                        else top->height = maxHeight;
                        break;
                    case InputType::MoveRight:
                        if (top->width < maxWidth) top->width++;
                        else top->width = maxWidth;
                        break;
                    case InputType::MoveLeft:
                        if (top->width > minWidth) top->width--;
                        break;
                    case InputType::R: case InputType::Enter:
                        top->isResizing = false; break;
                    case InputType::Escape:
                        top->isResizing = false; CycleWindow(); break;
                    default: break;
                    }
                }
                else {
                    switch (input) {
                    case InputType::Escape:
                        CycleWindow();
                        break;
                    case InputType::X:
                        RemoveWindow(top);
                        break;
                    case InputType::E:
                        top->isMoving = true;
                        break;
                    case InputType::Q:
                        top->isMinimized = true;
                        CycleWindow();
                        break;
                    case InputType::R:
                        // don't allow resizing of the start menu
                        if (top != startMenu) {
                            top->isResizing = true;
                            CycleWindow();
                        }
                        break;
                    default:
                        top->HandleInput(input);
                        break;
                    }
                }
            }
            else if (input == InputType::Escape) {
                running = false;
            }
        }
        std::cout << "\033[?1000l\033[?1002l\033[?1006l";
        std::cout << "\033[?25h\033[0m\033[2J\033[H" << std::flush;
    }
};