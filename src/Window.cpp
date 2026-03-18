#include <Window.hpp>
#include <init.hpp>
#include <windows/AlertWindow.hpp>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <thread>
#include <memory>

Window::Window(std::string t, int w, int h) : title(t), width(w), height(h), startWidth(w), startHeight(h) {
    x = (getConsoleWidth() - w) / 2;
    y = (getConsoleHeight() - h) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    oldX = x;
    oldY = y;
}
void Window::AddWidget(std::unique_ptr<Widget> w) {
    height = std::max(height, (int)widgets.size() + 5);
    widgets.push_back(std::move(w));

    if (focusedWidget == -1) {
        focusedWidget = 0;
        widgets[0]->focused = true;
    }
}
std::string Window::headerColor() {
    if (isResizing)
        return "\033[95;45m";
    else if (isMoving)
        return "\033[93;43m";
    else
        return focused ? "\033[97;44m" : "\033[37;100m";
}
void Window::Draw(std::ostream& buffer) {
    if (!visible || isMinimized) return;
    std::string resetStyle = "\033[0m";
    std::string shadowColor = "\033[48;2;45;45;45m";
    int termWidth = getConsoleWidth();
    if (!isMaximized) {
        for (int i = 1; i <= height; ++i) {
            int shadowX = x + 3;
            int shadowY = y + i + 1;
            if (shadowX > termWidth) continue;
            int availableWidth = termWidth - shadowX + 1;
            int shadowWidth = (std::min)(width, availableWidth);
            if (shadowWidth > 0) {
                buffer << "\033[" << shadowY << ";" << shadowX << "H" << shadowColor << std::string(shadowWidth, ' ') << resetStyle;
            }
        }
    }
    int innerWidth = width - 2;
    const int iconVisualWidth = 11;
    std::string silverBody = "\033[38;2;0;0;0;48;2;192;192;192m";
    auto drawLine = [&](int len) {
        std::string line;
        for (int i = 0; i < len; i++) line += "─";
        return line;
        };
    buffer << "\033[" << (y + 1) << ";" << (x + 1) << "H" << headerColor() << "┌" << drawLine(innerWidth) << "┐" << resetStyle;
    buffer << "\033[" << (y + 2) << ";" << (x + 1) << "H" << headerColor() << "│ ";
    std::string titleText = title;
    int spacing = (std::max)(0, innerWidth - (int)titleText.length() - iconVisualWidth - 1);
    buffer << titleText << std::string(spacing, ' ') << "[─] [O] [╳]│" << resetStyle;
    buffer << "\033[" << (y + 3) << ";" << (x + 1) << "H" << headerColor() << "├" << drawLine(innerWidth) << "┤" << resetStyle;
    for (int i = 3; i < height - 1; ++i) {
        buffer << "\033[" << (y + i + 1) << ";" << (x + 1) << "H" << silverBody << "│" << std::string(innerWidth, ' ') << "│" << resetStyle;
    }
    buffer << "\033[" << (y + height) << ";" << (x + 1) << "H" << silverBody << "└" << drawLine(innerWidth) << "┘" << resetStyle;
    for (auto& w : widgets)
        w->Draw(buffer, x + 1, y + 3);
}


void Window::Resize(int newW, int newH) {
    this->width = newW;
    this->height = newH;
    for (auto& w : widgets) {
        w->OnResize(this->width, this->height);
    }
}

void Window::ToggleMaximize(int screenWidth, int screenHeight) {
    if (!isMaximized) {
        oldX = x;
        oldY = y;
        oldWidth = width;
        oldHeight = height;
        x = 0;
        y = 0;
        width = screenWidth;
        height = screenHeight - 1;
        isMaximized = true;
    }
    else {
        x = oldX;
        y = oldY;
        width = oldWidth;
        height = oldHeight;
        isMaximized = false;
    }
}

void Window::HandleInput(InputType input) {
    if (widgets.empty()) return;
    if (input == InputType::MoveDown) {
        widgets[focusedWidget]->focused = false;
        int start = focusedWidget;
        do {
            focusedWidget = (focusedWidget + 1) % widgets.size();
        } 
        while (!widgets[focusedWidget]->focusable && focusedWidget != start);
        widgets[focusedWidget]->focused = true;
        return;
    }
    if (input == InputType::MoveUp) {
        widgets[focusedWidget]->focused = false;
        int start = focusedWidget;
        do {
            focusedWidget--;
            if (focusedWidget < 0) focusedWidget = widgets.size() - 1;
        } while (!widgets[focusedWidget]->focusable && focusedWidget != start);
        widgets[focusedWidget]->focused = true;
        return;
    }
    if (widgets[focusedWidget]->focusable) {
        widgets[focusedWidget]->HandleInput(input);
    }
}

bool Window::ContainsPoint(int px, int py) const {
    return px >= x && px < x + width &&
        py >= y && py < y + height;
}
void WindowManager::Alert(std::string message) {
    auto alert = std::make_shared<AlertWindow>(message, this);
    AddWindow(alert);
    alert->x = (getConsoleWidth() - alert->width) / 2;
    alert->y = (getConsoleHeight() - alert->height) / 2;
}
void WindowManager::exit(int code) {
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
#endif
    std::cout << "\033[?25h" << "\033[?1000l" << "\033[?1002l" << "\033[?1006l" << "\033[?1049l" << "\033[2J" << "\033[H" << "\033[!p" << std::flush;
    std::exit(code);
}
void WindowManager::SetStartMenu(std::shared_ptr<Window> sm) {
    startMenu = sm;
}
void WindowManager::AddWindow(std::shared_ptr<Window> w) {
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
void WindowManager::RemoveWindow(std::shared_ptr<Window> w) {
    auto it = std::find(windows.begin(), windows.end(), w);
    if (it != windows.end()) {
        windows.erase(it);
        windowCount--;
    }
}
void WindowManager::RemoveWindow(Window* w) {
    auto it = std::find_if(windows.begin(), windows.end(),
        [w](const std::shared_ptr<Window>& ptr) { return ptr.get() == w; });
    if (it != windows.end()) {
        windows.erase(it);
        windowCount--;
    }
}
void WindowManager::CycleWindow() {
    if (windows.size() < 2) return;
    auto top = windows.back();
    top->isMoving = false;
    windows.pop_back();
    windows.insert(windows.begin(), top);
    if (windows.back()->visible && windows.back()->isMinimized)
        windows.back()->isMinimized = false;
}
void WindowManager::Run() {
    init();
    std::cout << "\033[2J\033[?25l" << std::flush;
    std::string wallpaperColor = "\033[48;5;30m";
    bool running = true;
    static auto lastClickTime = std::chrono::steady_clock::now();
    static int lastClickX = -1;
    static int lastClickY = -1;
    const int DOUBLE_CLICK_DELAY = 500;
    while (running) {
        int sw = getConsoleWidth();
        int sh = getConsoleHeight();
        std::shared_ptr<Window> top = nullptr;
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
        frame << "\033[H";
        frame << "\033[48;5;30m";
        for (int i = 1; i <= sh; i++) {
            frame << "\033[" << i << ";1H\033[K";
        }
        for (auto& w : windows) {
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
        if (top && top->focusedWidget >= 0 &&
            top->focusedWidget < (int)top->widgets.size()) {

            Widget* w = top->widgets[top->focusedWidget].get();

            if (w->WantsRawInput()) {
                w->HandleRawInput();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }
        }
        InputType input = GetPlayerInput();
        if (input == InputType::MouseLeftDown) {
            int mx = getMouseX();
            int my = getMouseY();
            if (my == sh - 1) {
                if (mx >= 0 && mx <= 10) {
                    if (startMenu) {
                        startMenu->visible = !startMenu->visible;
                        if (startMenu->visible) {
                            auto it = std::find(windows.begin(), windows.end(), startMenu);
                            if (it != windows.end()) {
                                windows.erase(it);
                                windows.push_back(startMenu);
                            }
                            else {
                                windows.push_back(startMenu);
                            }
                        }
                        continue;
                    }
                }
                int currentX = 17;
                for (int i = 0; i < (int)windows.size(); i++) {
                    if (!windows[i]->visible) continue;
                    std::string label = std::to_string(i + 1) + ": " + windows[i]->title +
                        (windows[i]->isMinimized ? " (min)" : "");
                    int btnWidth = (int)label.length() + 4;
                    if (mx >= currentX && mx < currentX + btnWidth) {
                        auto selected = windows[i];
                        if (selected->focused && !selected->isMinimized) {
                            selected->isMinimized = true;
                            CycleWindow();
                        }
                        else {
                            selected->isMinimized = false;
                            windows.erase(windows.begin() + i);
                            windows.push_back(selected);
                        }
                        break;
                    }
                    currentX += btnWidth;
                }
                continue;
            }
            int found = -1;
            for (int i = (int)windows.size() - 1; i >= 0; --i) {
                if (!windows[i]->visible || windows[i]->isMinimized) continue;
                if (windows[i]->ContainsPoint(mx, my)) {
                    found = i;
                    break;
                }
            }
            if (found != -1) {
                auto wptr = windows[found];
                Window* w = wptr.get();
                auto now = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count();
                bool isSameSpot = (mx == lastClickX && my == lastClickY);
                bool isFastEnough = (duration < DOUBLE_CLICK_DELAY);
                bool isOnTitleBar = (my == w->y + 1 && mx >= w->x && mx < w->x + w->width);
                if (isSameSpot && isFastEnough && isOnTitleBar) {
                    w->ToggleMaximize(sw, sh);
                    lastClickX = -1;
                    continue;
                }
                lastClickTime = now;
                lastClickX = mx;
                lastClickY = my;
                if (found != (int)windows.size() - 1) {
                    windows.erase(windows.begin() + found);
                    windows.push_back(wptr);
                }
                for (auto& ww : windows) ww->focused = false;
                windows.back()->focused = true;
                const int iconVisualWidth = 11;
                int iconsStartX = w->x + w->width - 1 - iconVisualWidth;
                int topRow = w->y;
                int headerContentY = w->y + 1;
                int bottomRow = w->y + w->height - 1;
                if (my == headerContentY && mx >= iconsStartX && mx < iconsStartX + iconVisualWidth) {
                    int relx = mx - iconsStartX;
                    if (relx >= 8 && relx <= 10) {
                        RemoveWindow(wptr);
                        continue;
                    }
                    else if (relx >= 4 && relx <= 6) {
                        w->ToggleMaximize(sw, sh);
                        continue;
                    }
                    else if (relx >= 0 && relx <= 2) {
                        w->isMinimized = true;
                        CycleWindow();
                        continue;
                    }
                    else {
                        continue;
                    }
                }
                if (w->isMaximized) {
                    bool clickingHeader = (my == w->y + 1 && mx >= w->x && mx < w->x + w->width);
                    bool clickingResize = (mx == w->x || mx == w->x + w->width - 1 ||
                        my == w->y || my == w->y + w->height - 1);
                    if (clickingHeader || clickingResize) {
                        w->ToggleMaximize(sw, sh);
                        if (clickingHeader) {
                            w->dragOffsetX = mx - w->x;
                            w->dragOffsetY = my - w->y;
                        }
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
                auto wptr = windows.back();
                Window* w = wptr.get();
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
                    int dx = mx - w->resizeStartMouseX;
                    int dy = my - w->resizeStartMouseY;
                    int newX = w->resizeStartX;
                    int newY = w->resizeStartY;
                    int newW = w->resizeStartWidth;
                    int newH = w->resizeStartHeight;
                    if (w->resizeFlags & Window::RF_Right) {
                        newW = w->resizeStartWidth + dx;
                        if (newW < w->startWidth) newW = w->startWidth;
                        if (newX + newW > sw) newW = sw - newX;
                    }
                    if (w->resizeFlags & Window::RF_Left) {
                        newW = w->resizeStartWidth - dx;
                        newX = w->resizeStartX + dx;
                        if (newW < w->startWidth) {
                            newX = w->resizeStartX + (w->resizeStartWidth - w->startWidth);
                            newW = w->startWidth;
                        }
                        if (newX < 0) {
                            newW += newX;
                            newX = 0;
                            if (newW < w->startWidth) newW = w->startWidth;
                        }
                    }
                    if (w->resizeFlags & Window::RF_Bottom) {
                        newH = w->resizeStartHeight + dy;
                        if (newH < w->startHeight) newH = w->startHeight;
                        if (newY + newH > sh - 1) newH = (sh - 1) - newY;
                    }
                    if (w->resizeFlags & Window::RF_Top) {
                        newH = w->resizeStartHeight - dy;
                        newY = w->resizeStartY + dy;
                        if (newH < w->startHeight) {
                            newY = w->resizeStartY + (w->resizeStartHeight - w->startHeight);
                            newH = w->startHeight;
                        }
                        if (newY < 0) {
                            newH += newY;
                            newY = 0;
                            if (newH < w->startHeight) newH = w->startHeight;
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
            for (auto& w : windows) {
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
                auto selected = windows[targetIdx];
                windows.erase(windows.begin() + targetIdx);
                windows.push_back(selected);
            }
        }
        else if (top) {
            if (top->isMoving == true) {
                if (top->isMaximized) {
                    top->isMoving = false;
                }
                else {
                    switch (input) {
                    case InputType::MoveUp:    top->y--; break;
                    case InputType::MoveDown:  top->y++; break;
                    case InputType::MoveRight: top->x++; break;
                    case InputType::MoveLeft:  top->x--; break;
                    case InputType::R:
                        if (top.get() != startMenu.get()) {
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
            }
            else if (top->isResizing == true) {
                if (top->isMaximized) {
                    top->isResizing = false;
                }
                else {
                    int minHeight = 3;
                    int minWidth = 4;
                    int maxHeight = sh - top->y - 1;
                    int maxWidth = sw - top->x;
                    if (maxHeight < minHeight) maxHeight = minHeight;
                    if (maxWidth < minWidth) maxWidth = minWidth;
                    switch (input) {
                    case InputType::MoveUp:
                        if (top->height > top->startHeight) top->height--;
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
                        if (top->width > top->startWidth) top->width--;
                        break;
                    case InputType::R: case InputType::Enter:
                        top->isResizing = false; break;
                    case InputType::E:
                        top->isResizing = false; top->isMoving = true; break;
                    case InputType::Escape:
                        top->isResizing = false; CycleWindow(); break;
                    default: break;
                    }
                }
            }
            else {
                switch (input) {
                case InputType::Escape:
                    CycleWindow();
                    break;
                case InputType::X:
                    RemoveWindow(top.get());
                    break;
                case InputType::E:
                    if (!top->isMaximized) top->isMoving = true;
                    break;
                case InputType::Q:
                    top->isMinimized = true;
                    CycleWindow();
                    break;
                case InputType::R:
                    if (!top->isMaximized) top->isResizing = true;
                    break;
                case InputType::Z:
                    top->ToggleMaximize(sw, sh);
                    break;
                default:
                    top->HandleInput(input);
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    exit(1);
}