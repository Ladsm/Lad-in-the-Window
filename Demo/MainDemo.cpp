#include <LITW.hpp>
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
            wm.RemoveWindow(this);
            }));
    }

    TextWindow(std::string title, std::string msg, int w, int h)
        : Window(title, w, h) {
        AddWidget(std::make_unique<Label>(2, 2, msg));
        AddWidget(std::make_unique<Button>(2, 5, "Close", [this]() {
            wm.RemoveWindow(this);
            }));
    }
};
class README : public Window {
    std::vector<std::string> text = {
        "Lad in the Window, Ladsm 2026",
        "version : " LITWVERSION,
        "-----------------------------",
        "Hot keys:",
        "Space: Scroll Down on scrollables.",
        "Tab: Scroll up on scrollables.",
        "Enter: confirm input and tick check box.",
        "WASD and Arrow keys: Movement.",
        "Q: Minimize window  X: Close window.",
        "E: Edit window position  R: Resize window.",
        "C: Start menu  Escape: Cycle windows.",
        "1-9: Change window by window number(taskbar number).",
        "F12: Exit LITW.",
        "---------------------------------------------------",
        "Hello, Ladsm here.",
        "This program is a demo of the Lad in a Window TUI",
        "framework. This TUI is made to look like",
        "a GUI."
    };
public:
    README() : Window("README", 60, 18) {
        AddWidget(std::make_unique<ScrollableTextBox>(2, 2, 10, 55, text));
        AddWidget(std::make_unique<Button>(2, 14, "Close", [this]() {
            wm.RemoveWindow(this);
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
        AddWidget(std::make_unique<Button>(2, 8, "Close", [this]() { wm.RemoveWindow(this); }));
    }
};
class ShellWindow : public Window {
public:
    ShellWindow() : Window("Terminal", 80, 24) {}
    static std::shared_ptr<ShellWindow> Create() {
        auto win = std::make_shared<ShellWindow>();
        win->AddWidget(std::make_unique<ShellWidget>(1, 1, win));
        return win;
    }
};
bool a = true, b = true;
class ScrollExample : public Window {
public:
    ScrollExample() : Window("Scroll Example", 40, 10) {
        auto container = std::make_unique<ScrollContainer>(2, 2, 30, 3);
        container->AddWidget(std::make_unique<CheckBox>(0, 1, "State one", a));
        container->AddWidget(std::make_unique<Label>(0, 3, "middle"));
        container->AddWidget(std::make_unique<CheckBox>(0, 5, "State two", b));
        container->AddWidget(std::make_unique<Button>(0, 7, "Save Settings", []() { wm.Alert("Saved!"); }));
        AddWidget(std::move(container));
        AddWidget(std::make_unique<Button>(2, 6, "Close", [this]() { wm.RemoveWindow(this); }));
    }
};
class TextInputLargeDemo : public Window {
    std::vector<std::string> text = {
        "#include <iostream>",
        "int main() {",
        "    std::cout << 123 << \"test\" << '\\n';",
        "}"
    };
public:
    TextInputLargeDemo() : Window("Text input - large", 50, 20) {
        AddWidget(std::make_unique<LargeTextInput>(2, 2, 10, 40, &text, this));
        AddWidget(std::make_unique<Button>(2, 14, "dsaf", [this]() { std::cout << "dsafklasd"; }));
        AddWidget(std::make_unique<Button>(2, 15, "Close", [this]() { wm.RemoveWindow(this); }));
    }
};
class Contest : public Window {
public:
    Contest() : Window("Container test", 50, 20) {
        auto vbox = std::make_unique<VertCon>(2, 2);
        vbox->AddWidget(std::make_unique<Label>(0, 0, "Settings"));
        vbox->AddWidget(std::make_unique<Button>(0, 0, "Apply", [] {}));
        vbox->AddWidget(std::make_unique<Button>(0, 0, "Cancel", [] {}));
        AddWidget(std::move(vbox));
        auto hbox = std::make_unique<HorizlCon>(2, 7);
        hbox->AddWidget(std::make_unique<Button>(0, 0, "Yes", [] {}));
        hbox->AddWidget(std::make_unique<Button>(0, 0, "No", [] {}));
        hbox->AddWidget(std::make_unique<Button>(0, 0, "Cancel", [] {}));
        AddWidget(std::move(hbox));
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
    start->AddItem("Scroll Example", []() {
        return std::make_shared<ScrollExample>();
        });
    start->AddItem("Text input demo - Large", []() {
        return std::make_shared<TextInputLargeDemo>();
        });
    start->AddItem("Containers", []() {
        return std::make_shared<Contest>();
        });
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}