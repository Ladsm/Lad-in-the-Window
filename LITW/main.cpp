#include "getwh.hpp"
#include "Window.hpp"
#include "init.hpp"
#include "StartMenu.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include <iostream>
#include <functional>

class MenuWindow : public Window {
public:
    MenuWindow(std::string title) : Window(title, 40, 10) {
        AddWidget(new Button(2, 2, "Exit", [this]() {
            std::exit(0);
            }
        ));
    }
};
class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg) : Window(title, 40, 10) {
        AddWidget(new Label(2, 2, msg));
        AddWidget(new Button(2, 5, "Close", [this]() {
            visible = false;
            }
        ));
    }
    TextWindow(std::string title, std::string msg, int w, int h) : Window(title, w, h) {
        AddWidget(new Label(2, 2, msg));
        AddWidget(new Button(2, 5, "Close", [this]() {
            visible = false;
            }
        ));
    }
};
class README : public Window {
public:
    README() : Window("README", 40, 10) {
        AddWidget(new Label(2, 2, "Lad in the Window, Ladsm 2026"));
        AddWidget(new Label(2, 3, "-----------------------------"));
        AddWidget(new Label(2, 4, "uhhhhh... enjoy the GUI in a TUI."));
        AddWidget(new Button(2, 5, "Close", [this]() {
            visible = false;
            }
        ));
    }
};

int main() {
    init();
    WindowManager wm;
    StartMenuWindow start(&wm);
    start.AddItem("Main Menu", []() {
        return new MenuWindow("main menu");
        });
    start.AddItem("System Status", []() {
        return new TextWindow("System status", "System is running smoothly!");
        });
    start.AddItem("README", []() {
        return new README();
        });
    wm.SetStartMenu(&start);
    wm.AddWindow(&start);
    wm.Run();
    std::cout << "\033[?1000l\033[?1002l\033[?1006l";
    std::cout << "\033[?25h\033[0m\033[2J\033[H";
    std::cout << "\033[?1049l" << std::flush;
}