#include "getwh.hpp"
#include "Window.hpp"
#include "init.hpp"
#include "StartMenu.hpp"
#include <widgets/Button.hpp>
#include <widgets/Label.hpp>
#include <widgets/TextInput.hpp>
#include <widgets/LargeTextbox.hpp>
#include <widgets/Checkbox.hpp>
#include <widgets/Bar.hpp>
#include <widgets/table.hpp>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>

std::string title = "text entering demo";
bool globlestate = false;
WindowManager wm;
class Textinputer : public Window {
public:
    Textinputer() : Window(::title, 40, 10) {
        AddWidget(std::make_unique<TextInput>(2, 2, 20, &::title));
        AddWidget(std::make_unique<CheckBox>(2, 3, "global state?", globlestate));
        AddWidget(std::make_unique<Separator>(0, 4, this));
        AddWidget(std::make_unique<Button>(2, 5, "Exit", []() {
            std::cout << "\033[?1000l\033[?1002l\033[?1006l";
            std::cout << "\033[?25h\033[0m\033[2J\033[H";
            std::cout << "\033[?1049l" << std::flush;
            std::exit(0);
        }));
    }
    void Draw(std::ostream& buffer) override {
        if (this->title != ::title) {
            this->title = ::title;
        }
        Window::Draw(buffer);
    }
};
class MenuWindow : public Window {
public:
    MenuWindow(std::string title) : Window(title, 40, 10) {
        AddWidget(std::make_unique<Button>(2, 2, "Exit", []() {
            std::cout << "\033[?1000l\033[?1002l\033[?1006l";
            std::cout << "\033[?25h\033[0m\033[2J\033[H";
            std::cout << "\033[?1049l" << std::flush;
            std::exit(0);
            }));
    }
};

class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 10) {
        AddWidget(std::make_unique<Label>(2, 2, msg));
        AddWidget(std::make_unique<Button>(2, 5, "Close", [this]() {
            visible = false;
            }));
    }

    TextWindow(std::string title, std::string msg, int w, int h)
        : Window(title, w, h) {
        AddWidget(std::make_unique<Label>(2, 2, msg));
        AddWidget(std::make_unique<Button>(2, 5, "Close", [this]() {
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
        "C: Start menu  Escape: Cycle windows",
        "1-9: Change window by window number(taskbar number)"
        "",
        "Hello, Ladsm here.",
        "This program is a demo of the Lad in a Window TUI",
        "framework. This TUI is made to look like",
        "a GUI.",
        "LITW is made more as a art project then a real",
        "TUI framework."
    };
public:
    README() : Window("README", 55, 20) {
        AddWidget(std::make_unique<TextBox>(2, 2, text));
        AddWidget(std::make_unique<Button>(2, 16, "Close", [this]() {
            visible = false;
            wm.Alert("This is an alert");
        }));
    }
};
class Users : public Window {
public:
    Users() : Window("List of Users", 40, 12) {
        auto table = std::make_unique<Table>(2, 2,
            std::vector<std::string>{"ID", "Name", "Status"},
            std::vector<int>{4, 15, 10}
        );
        table->AddRow({ "01", "Ladsm", "Online" });
        table->AddRow({ "02", "Guest", "Offline" });
        AddWidget(std::move(table));
        AddWidget(std::make_unique<Button>(2, 8, "Close", [this]() { visible = false; }));
    }
};
int main() {
    init();
    auto start = std::make_shared<StartMenuWindow>(&wm);
    start->AddItem("Main Menu", []() {
        return std::make_shared<MenuWindow>("main menu");
        });
    start->AddItem("System Status", []() {
        return std::make_shared<TextWindow>("System status", "System is running smoothly!");
        });
    start->AddItem("README", []() {
        return std::make_shared<README>();
        });
    start->AddItem("Text Input Demo", []() {
        return std::make_shared<Textinputer>();
        });
    start->AddItem("Table of Users", []() {
        return std::make_shared<Users>();
        });
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    std::cout << "\033[?1000l\033[?1002l\033[?1006l";
    std::cout << "\033[?25h\033[0m\033[2J\033[H";
    std::cout << "\033[?1049l" << std::flush;
    return 0;
}