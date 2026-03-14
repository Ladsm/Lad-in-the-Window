#include "getwh.hpp"
#include "Window.hpp"
#include "init.hpp"
#include "StartMenu.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "TextInput.hpp"
#include "LargeTextbox.hpp"
#include <iostream>
#include <functional>
#include <cstdlib>

std::string title = "text entering demo";
class Textinputer : public Window {
public:
    Textinputer() : Window("title", 40, 10) {
        AddWidget(new TextInput(2, 2, 25, title));
        AddWidget(new Button(2, 3, "Exit", []() {
            std::exit(0);
        }));
    }
};
class MenuWindow : public Window {
public:
    MenuWindow(std::string title) : Window(title, 40, 10) {
        AddWidget(new Button(2, 2, "Exit", []() {
            std::exit(0);
        }));
    }
};

class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 10) {
        AddWidget(new Label(2, 2, msg));
        AddWidget(new Button(2, 5, "Close", [this]() {
            visible = false;
        }));
    }

    TextWindow(std::string title, std::string msg, int w, int h)
        : Window(title, w, h) {
        AddWidget(new Label(2, 2, msg));
        AddWidget(new Button(2, 5, "Close", [this]() {
            visible = false;
        }));
    }
};
class README : public Window {
    std::vector<std::string> text = {
        "Lad in the Window, Ladsm 2026",
        "-----------------------------",
        "Hot keys:",
        "WASD and Arrow keys: Movement",
        "Q: Minimize window  X: Close window",
        "E: Edit window position  R: Resize window",
        "C: Start menu",
        "",
        "Hello, Ladsm here.",
        "This program is a demo of the Lad in a Window TUI",
        "framework. This TUI is made to look like",
        "a GUI.",
        "LITW is made more as a art project then a real",
        "TUI framework,"
    };
public:
    README() : Window("README", 55, 20) {
        AddWidget(new TextBox(2, 2, text));
        AddWidget(new Button(2, 16, "Close", [this]() {
            visible = false;
        }));
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
    start.AddItem("Text Input Demo", []() {
        return new Textinputer();
        });
    wm.SetStartMenu(&start);
    wm.AddWindow(&start);
    wm.Run();
    std::cout << "\033[?1000l\033[?1002l\033[?1006l";
    std::cout << "\033[?25h\033[0m\033[2J\033[H";
    std::cout << "\033[?1049l" << std::flush;
    return 0;
}