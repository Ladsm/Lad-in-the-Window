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
    Textinputer() : Window(::title, 47, 8) {
        auto& vbox = Add<VertCon>(2, 2, 1);
        auto& hbox = vbox.Add<HorizCon>(0, 0);
        hbox.Add<TextInput>(0, 0, 20, &::title);
        hbox.Add<CheckBox>(0, 0, "global state?", globlestate);
        Add<Separator>(0, 3, this);
        vbox.Add<Button>(0, 0, "Close", [this]() { wm.RemoveWindow(this); });
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
        Add<Button>(2, 2, "Exit", []() {
            wm.exit(0);
            });
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
        auto& vbox = Add<VertCon>(2, 2);
        vbox.Add<ScrollableTextBox>(0, 0, 10, 55, text);
        vbox.Add<Button>(0, 0, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
class Users : public Window {
public:
    Users() : Window("List of Users", 40, 12) {
        auto& vbox = Add<VertCon>(2, 2);
        auto& table = vbox.Add<Table>(2, 2,
            std::vector<std::string>{"ID", "Name", "Status"},
            std::vector<int>{4, 15, 10}
        );
        table.AddRow({ "01", "Ladsm", "Online" });
        table.AddRow({ "02", "Guest", "Offline" });
        vbox.Add<Button>(0, 0, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
class ShellWindow : public Window {
public:
    ShellWindow() : Window("Terminal", 80, 24) {}
    static std::shared_ptr<ShellWindow> Create() {
        auto win = std::make_shared<ShellWindow>();
        win->Add<ShellWidget>(1, 1, win);
        return win;
    }
};
bool a = true, b = true;
class ScrollExample : public Window {
public:
    ScrollExample() : Window("Scroll Example", 40, 10) {
        auto container = std::make_unique<ScrollContainer>(2, 2, 30, 3);
        container->Add<CheckBox>(0, 1, "State one", a);
        container->Add<Label>(0, 3, "middle");
        container->Add<CheckBox>(0, 5, "State two", b);
        container->Add<Button>(0, 7, "Save Settings", []() { wm.Alert("Saved!"); });
        AddWidget(std::move(container));
        Add<Button>(2, 6, "Close", [this]() { wm.RemoveWindow(this); });
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
        Add<LargeTextInput>(2, 2, 10, 45, &text, this);
        Add<Button>(2, 15, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
class Contest : public Window {
    std::string text = "input here!";
public:
    Contest() : Window("Container test", 50, 11) {
        auto& vbox = Add<VertCon>(2, 2);
        vbox.Add<Label>(0, 0, "Settings");
        vbox.Add<Button>(0, 0, "Apply", [] {});
        auto& hbox = Add<HorizCon>(2, 7);
        hbox.Add<Button>(0, 0, "Yes", [] {});
        hbox.Add<Button>(0, 0, "no", [] {});
        hbox.Add<TextInput>(0, 0, 15, &text);
    }
};
class StartAlert : public Window {
public:
    StartAlert() : Window("Start Alert", 40, 10) {
        auto& vbox = Add<VertCon>(1, 2);
        vbox.Add<Label>(0,0, "Welcome to the Lad in the Window demo!");
        vbox.Add<Label>(0,0, "sep here"); Add<Separator>(0, 3, this);
        vbox.Add<Label>(0,0, "Check readme in the start menu for");
        vbox.Add<Label>(0,0, "more info.");
    }
};
class EyesWindow : public Window {
public:
    EyesWindow() : Window("Eyes", 30, 10) {
        AddWidget(std::make_unique<EyesWidget>(8, 2, 14, 4));
        auto& vbox = Add<VertCon>(2, 7);
        vbox.Add<Button>(0, 0, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
int main() {
    auto startalert = std::make_shared<StartAlert>();
    auto start = std::make_shared<StartMenuWindow>(&wm);
    start->AddItem<README>("README");
    start->AddItem<MenuWindow>("Main Menu", "main menu");
    start->AddItem<Textinputer>("Text Input Demo");
    start->AddItem<Users>("Table of Users");
    start->AddItem("Terminal", &ShellWindow::Create);
    start->AddItem<ScrollExample>("Scroll Example");
    start->AddItem<TextInputLargeDemo>("Text input demo - Large");
    start->AddItem<Contest>("Containers");
    start->AddItem<EyesWindow>("Eyes");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.AddWindow(startalert);
    wm.Run();
    wm.exit(1);
    return 0;
}
