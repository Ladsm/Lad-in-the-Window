#include <LITW.hpp>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>

WindowManager wm;
class Textinputer : public Window {
    std::string text = "Text Entering Demo";
    bool checkbox = false;
public:
    Textinputer() : Window("Text Entering Demo", 47, 8) {
        auto& vbox = Add<VerticalContainer>(2, 2, 0);
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<TextInput>(20, &text);
        hbox.Add<CheckBox>("checkbox", checkbox);
        vbox.Add<Separator>(this);
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
class MenuWindow : public Window {
public:
    MenuWindow(std::string title) : Window(title, 40, 10) {
        auto& vbox = Add<VerticalContainer>(2, 2);
        vbox.Add<Button>("Exit", []() {
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
        auto& vbox = Add<VerticalContainer>(2, 2);
        vbox.Add<ScrollableTextBox>(10, 55, text);
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
class Users : public Window {
public:
    Users() : Window("List of Users", 40, 12) {
        auto& vbox = Add<VerticalContainer>(2, 2);
        auto& table = vbox.Add<Table>(
            std::vector<std::string>{"ID", "Name", "Status"},
            std::vector<int>{4, 15, 10}
        );
        table.AddRow({ "01", "Ladsm", "Online" });
        table.AddRow({ "02", "Guest", "Offline" });
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
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
        container->Add<Label>(0, 3, "Middle");
        container->Add<CheckBox>(0, 5, "State Two", b);
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
    TextInputLargeDemo() : Window("Text Input - Large", 50, 20) {
        Add<LargeTextInput>(2, 2, 10, 45, &text, this);
        Add<Button>(2, 15, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
class ContainerTest: public Window {
    std::string text = "Input Here!";
    bool offon = false;
public:
    ContainerTest() : Window("Container Test", 40, 10) {
        auto& vbox = Add<VerticalContainer>(2, 2, 0);
        vbox.Add<Label>("Settings");
        vbox.Add<Button>("Apply", [] {});
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Button>("Yes", [] {});
        hbox.Add<Button>("No", [] {});
        hbox.Add<TextInput>(15, &text);
        vbox.Add<Toggle>("Toggle", offon);
    }
};
class StartAlert : public Window {
public:
    StartAlert() : Window("Start Alert", 40, 10) {
        auto& vbox = Add<VerticalContainer>(1, 2);
        vbox.Add<Label>("Welcome to the Lad in the Window demo!");
        vbox.Add<Separator>(this);
        vbox.Add<Label>("Check readme in the start menu for");
        vbox.Add<Label>("more info.");
    }
};
class EyesWindow : public Window {
public:
    EyesWindow() : Window("Eyes", 30, 10) {
        auto& vbox = Add<VerticalContainer>(8, 2);
        vbox.Add<EyesWidget>(15, 5);
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
int main() {
    auto startalert = mksharedWindow<StartAlert>();
    auto start = startmenu<StartMenuWindow>(&wm);
    start->AddItem<README>("README");
    start->AddItem<MenuWindow>("Main Menu", "main menu");
    start->AddItem<Textinputer>("Text Input Demo");
    start->AddItem<Users>("Table of Users");
    start->AddItem("Terminal", &ShellWindow::Create);
    start->AddItem<ScrollExample>("Scroll Example");
    start->AddItem<TextInputLargeDemo>("Text input demo - Large");
    start->AddItem<ContainerTest>("Containers");
    start->AddItem<EyesWindow>("Eyes");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.AddWindow(startalert);
    wm.Run();
    wm.exit(1);
    return 0;
}
