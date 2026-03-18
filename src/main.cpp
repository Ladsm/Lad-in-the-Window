#include "getwh.hpp"
#include "Window.hpp"
#include <windows/StartMenu.hpp>
#include <widgets/Button.hpp>
#include <widgets/text/Label.hpp>
#include <widgets/text/TextInput.hpp>
#include <widgets/text/LargeTextbox.hpp>
#include <widgets/text/Scrollable.hpp>
#include <widgets/Checkbox.hpp>
#include <widgets/Bar.hpp>
#include <widgets/table.hpp>
#include <widgets/ShellWidget.hpp>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>

std::string title = "Text Entering Demo";
bool globlestate = false;
WindowManager wm;
class Textinputer : public Window {
public:
    Textinputer() : Window(::title, 40, 10) {
        AddWidget(std::make_unique<TextInput>(2, 2, 20, &::title));
        AddWidget(std::make_unique<CheckBox>(2, 3, "global state?", globlestate));
        AddWidget(std::make_unique<Separator>(0, 4, this));
        AddWidget(std::make_unique<Button>(2, 5, "Exit", []() {
            wm.exit(0);
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
            wm.exit(0);
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
        "Lad i n the Window, Ladsm 2026",
        "-----------------------------",
        "Hot keys:",
        "Space: Scroll Down on scrollables",
        "Tab: Scroll up on scrollables",
        "Enter: confirm input and tick check box",
        "WASD and Arrow keys: Movement",
        "Q: Minimize window  X: Close window",
        "E: Edit window position  R: Resize window",
        "C: Start menu  Escape: Cycle windows",
        "1-9: Change window by window number(taskbar number)",
        "---------------------------------------------------",
        "Hello, Ladsm here.",
        "This program is a demo of the Lad in a Window TUI",
        "framework. This TUI is made to look like",
        "a GUI."
    };
public:
    README() : Window("README", 60, 18) {
        AddWidget(std::make_unique<ScrollableTextBox>(2, 2, 10, 54, text));
        AddWidget(std::make_unique<Button>(2, 13, "Close", [this]() {
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
class ShellWindow : public Window {
public:
    ShellWindow() : Window("Terminal", 80, 24) {}
    static std::shared_ptr<ShellWindow> Create() {
        auto win = std::make_shared<ShellWindow>();
        win->AddWidget(
            std::make_unique<ShellWidget>(1, 1, 76, 20, win)
        );
        return win;
    }
};
int main() {
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
    start->AddItem("Terminal", []() {
        return ShellWindow::Create();
        });
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}