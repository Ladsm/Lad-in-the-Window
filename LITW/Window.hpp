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
    Window(std::string t, int w, int h) : title(t), width(w), height(h) {
        x = (getConsoleWidth() - w) / 2;
        y = (getConsoleHeight() - h) / 2;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
    }
    std::string headerColor() {
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
    bool ContainsPoint(int px, int py) const {
        return px >= x && px < x + width && py >= y && py < y + height;
    }
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
                if (windows[i]->x < 0) windows[i]->x = 0;
                if (windows[i]->x + windows[i]->width > sw)
                    windows[i]->x = sw - windows[i]->width;
                if (windows[i]->y < 0) windows[i]->y = 0;
                if (windows[i]->y + windows[i]->height > sh - 1)
                    windows[i]->y = (sh - 1) - windows[i]->height;
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
            if (input == InputType::MouseLeftDown) {
                int mx = getMouseX();
                int my = getMouseY();
                int found = -1;
                for (int i = (int)windows.size() - 1; i >= 0; --i) {
                    if (!windows[i]->visible || windows[i]->isMinimized) continue;
                    if (windows[i]->ContainsPoint(mx, my)) {
                        found = i;
                        break;
                    }
                }
                if (found != -1) {
                    Window* w = windows[found];
                    if (found != (int)windows.size() - 1) {
                        windows.erase(windows.begin() + found);
                        windows.push_back(w);
                    }
                    for (auto* ww : windows) ww->focused = false;
                    windows.back()->focused = true;

                    const int iconVisualWidth = 11;
                    int iconsStartX = w->x + w->width - 1 - iconVisualWidth;
                    int topRow = w->y;
                    int headerContentY = w->y + 1;
                    int bottomRow = w->y + w->height - 1;
                    if (my == headerContentY && mx >= iconsStartX && mx < iconsStartX + iconVisualWidth) {
                        int relx = mx - iconsStartX;
                        if (relx >= 8 && relx <= 10) {
                            RemoveWindow(w);
                            continue;
                        }
                        else if (relx >= 4 && relx <= 6) {
                            w->isMinimized = true;
                            CycleWindow();
                            continue;
                        }
                        else {
                            continue;
                        }
                    }
                    int rf = Window::RF_None;
                    bool onLeft = (mx == w->x);
                    bool onRight = (mx == w->x + w->width - 1);
                    bool onTop = (my == topRow);
                    bool onBottom = (my == bottomRow);

                    if ((onLeft && onTop)) rf = Window::RF_Left | Window::RF_Top;
                    else if ((onRight && onTop)) rf = Window::RF_Right | Window::RF_Top;
                    else if ((onLeft && onBottom)) rf = Window::RF_Left | Window::RF_Bottom;
                    else if ((onRight && onBottom)) rf = Window::RF_Right | Window::RF_Bottom;
                    else if (onLeft) rf = Window::RF_Left;
                    else if (onRight) rf = Window::RF_Right;
                    else if (onTop) rf = Window::RF_Top;
                    else if (onBottom) rf = Window::RF_Bottom;

                    if (rf != Window::RF_None) {
                        w->isResizing = true;
                        w->resizeFlags = rf;
                        w->resizeStartMouseX = mx;
                        w->resizeStartMouseY = my;
                        w->resizeStartWidth = w->width;
                        w->resizeStartHeight = w->height;
                        w->resizeStartX = w->x;
                        w->resizeStartY = w->y;
                        continue;
                    }
                    if (my == headerContentY && mx >= w->x && mx < w->x + w->width) {
                        w->isMoving = true;
                        w->dragOffsetX = mx - w->x;
                        w->dragOffsetY = my - w->y;
                        continue;
                    }
                    continue;
                }
            }
            else if (input == InputType::MouseMove) {
                int mx = getMouseX();
                int my = getMouseY();
                if (!windows.empty()) {
                    Window* w = windows.back();
                    if (w->isMoving) {
                        w->x = mx - w->dragOffsetX;
                        w->y = my - w->dragOffsetY;
                        if (w->x < 0) w->x = 0;
                        if (w->y < 0) w->y = 0;
                        if (w->x + w->width > sw) w->x = sw - w->width;
                        if (w->y + w->height > sh - 1) w->y = (sh - 1) - w->height;
                        continue;
                    }
                    if (w->isResizing && w->resizeFlags != Window::RF_None) {
                        const int minWidth = 4;
                        const int minHeight = 3;

                        int dx = mx - w->resizeStartMouseX;
                        int dy = my - w->resizeStartMouseY;

                        int newX = w->resizeStartX;
                        int newY = w->resizeStartY;
                        int newW = w->resizeStartWidth;
                        int newH = w->resizeStartHeight;
                        if (w->resizeFlags & Window::RF_Right) {
                            newW = w->resizeStartWidth + dx;
                            if (newW < minWidth) newW = minWidth;
                            if (newX + newW > sw) newW = sw - newX;
                        }
                        if (w->resizeFlags & Window::RF_Left) {
                            newW = w->resizeStartWidth - dx;
                            newX = w->resizeStartX + dx;
                            if (newW < minWidth) {
                                newX = w->resizeStartX + (w->resizeStartWidth - minWidth);
                                newW = minWidth;
                            }
                            if (newX < 0) {
                                newW += newX;
                                newX = 0;
                                if (newW < minWidth) newW = minWidth;
                            }
                        }
                        if (w->resizeFlags & Window::RF_Bottom) {
                            newH = w->resizeStartHeight + dy;
                            if (newH < minHeight) newH = minHeight;
                            if (newY + newH > sh - 1) newH = (sh - 1) - newY;
                        }
                        if (w->resizeFlags & Window::RF_Top) {
                            newH = w->resizeStartHeight - dy;
                            newY = w->resizeStartY + dy;
                            if (newH < minHeight) {
                                newY = w->resizeStartY + (w->resizeStartHeight - minHeight);
                                newH = minHeight;
                            }
                            if (newY < 0) {
                                newH += newY;
                                newY = 0;
                                if (newH < minHeight) newH = minHeight;
                            }
                        }
                        w->x = newX;
                        w->y = newY;
                        w->width = newW;
                        w->height = newH;
                        if (w->x < 0) w->x = 0;
                        if (w->y < 0) w->y = 0;
                        if (w->width > sw) w->width = sw;
                        if (w->height > sh - 1) w->height = sh - 1;

                        continue;
                    }
                }
            }
            else if (input == InputType::MouseLeftUp) {
                for (auto* w : windows) {
                    if (w->isMoving) w->isMoving = false;
                    if (w->isResizing) {
                        w->isResizing = false;
                        w->resizeFlags = Window::RF_None;
                    }
                }
                continue;
            }
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
                        top->isResizing = true;
                        CycleWindow();
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