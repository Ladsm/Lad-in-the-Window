#include <LITW/LITW.hpp>
#include <iostream>
#include <functional>
#include <cstdlib>

litw::WindowManager wm("LITW demo");
class Textinputer : public litw::Window {
    std::string text = "Text Entering Demo";
    bool checkbox = false;
public:
    Textinputer() : litw::Window("Text Entering Demo", 47, 8) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 0);
        auto& hbox = vbox.Add<litw::HorizontalContainer>();
        hbox.Add<litw::TextInput>(20, &text);
        hbox.Add<litw::CheckBox>("checkbox", checkbox);
        vbox.Add<litw::HorizontalSeparator>();
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
class MenuWindow : public litw::Window {
public:
    MenuWindow(std::string title) : litw::Window(title, 40, 10) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2);
        vbox.Add<litw::Button>("Exit", []() {
            wm.exit(0);
            });
    }
};
class README : public litw::Window {
    std::vector<std::string> text = {
        "Lad in the Window, Ladsm 2026",
        "version : " LITWVERSION_STRING,
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
        auto& vbox = Add<litw::VerticalContainer>(2, 2);
        vbox.Add<litw::ScrollableTextBox>(10, 55, text);
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

class Users : public litw::Window {
public:
    Users() : litw::Window("List of Users", 40, 12) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2);
        auto& table = vbox.Add<litw::Table>(
            std::vector<std::string>{"ID", "Name", "Status"},
            std::vector<int>{4, 15, 10}
        );
        table.AddRow({ "01", "Ladsm", "Online" });
        table.AddRow({ "02", "Guest", "Offline" });
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
class ShellWindow : public litw::Window {
public:
    ShellWindow() : litw::Window("Terminal", 80, 24) {}
    static std::shared_ptr<ShellWindow> Create() {
        auto win = std::make_shared<ShellWindow>();
        win->Add<litw::ShellWidget>(1, 1, win);
        return win;
    }
};

litw::WindowPalette winpal = litw::WindowPalette(
    "\033[38;2;255;255;255;48;2;37;0;100m",
    "\033[38;2;255;255;255;48;2;102;49;0m",
    "\033[38;2;255;255;255;48;2;0;0;0m",
    "\033[38;2;255;255;255;48;2;84;84;84m",
    "\033[38;2;255;255;255;48;2;18;18;18m",
    "\033[48;2;45;45;45m"
);
class TextInputLargeDemo : public litw::Window {
    std::vector<std::string> text = {
        "#include <iostream>",
        "",
        "int main() {",
        "    int b = 123;",
        "    std::cout << b << \"test\" << '\\n';",
        "}"
    };
    litw::LargeTextInput* LTIWidght = nullptr;
public:
    TextInputLargeDemo() : litw::Window("Text Input - Large", 50, 20, winpal) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 1);
        LTIWidght = &vbox.Add<litw::LargeTextInput>(10, 45, &text, true);
        LTIWidght->createCppSnippets();
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
    void Draw(std::ostream& buffer) override {
        LTIWidght->height = height - 8;
        LTIWidght->width = width - 5;
        litw::Window::Draw(buffer);
    }
    /*
     * litw::Window::Draw is a function that runs every frame that a window is on screen.
     * You can use this to create dynamicly size changeing widgets like so.
     * Just remember to call Window::Draw(buffer); at the end so it actualy draws.
     */
};
class ContainerTest: public litw::Window {
    std::string text = "Input Here!";
    bool offon = false;
public:
    ContainerTest() : litw::Window("Container Test", 50, 10) {
        resizeable = false;
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 0);
        vbox.Add<litw::Label>("Settings");
        vbox.Add<litw::Button>("Apply", [] {});
        auto& hbox = vbox.Add<litw::HorizontalContainer>();
        hbox.Add<litw::Label>("Button example");
        hbox.Add<litw::Button>("Yes", [] {});
        hbox.Add<litw::Button>("No", [] {});
        hbox.Add<litw::TextInput>(15, &text);
        vbox.Add<litw::Toggle>("Toggle", offon);
        Add<litw::VerticalSeparator>(17);
    }
};
class StartAlert : public litw::Window {
public:
    StartAlert() : litw::Window("Start Alert", 40, 10) {
        auto& vbox = Add<litw::VerticalContainer>(1, 2);
        vbox.Add<litw::Label>("Welcome to the Lad in the Window demo!");
        vbox.Add<litw::HorizontalSeparator>();
        vbox.Add<litw::Label>("Check readme in the start menu for");
        vbox.Add<litw::Label>("more info.");
    }
};
class EyesWindow : public litw::Window {
public:
    EyesWindow() : litw::Window("Eyes", 30, 11) {
        auto& vbox = Add<litw::VerticalContainer>(8, 2);
        vbox.Add<litw::EyesWidget>(15, 5);
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};
class NumberInputWindow : public litw::Window {
    float number = 0;
public:
    NumberInputWindow() : litw::Window("Number Input Demo", 31, 7) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2);
        vbox.Add<litw::NumberInput<float>>(7, &number);
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

litw::WindowPalette TestPalette = litw::WindowPalette(
    "\033[1;2;0;0;0;48;2;255;128;227m", //Resize
    "\033[1;2;0;0;0;48;2;55;255;0m", //Move
    "\033[38;2;255;0;0;48;2;255;255;255m", //Focused
    "\033[38;2;255;255;255;48;2;255;0;0m", //Unfocused
    "\033[38;2;255;0;208;48;2;163;163;163m", //Body
    "\033[38;2;128;128;128;48;2;102;102;102m" //Shadow
);

class PaletteTest : public litw::Window {
public:
    PaletteTest() : litw::Window("Palette Test", 30, 7, TestPalette) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2);
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

class WindowWithSubwindow : public litw::Window {
    litw::Label* BoolSomething = nullptr;
    bool SubwindowBool = false;
public:
    WindowWithSubwindow() : litw::Window("Subwindow Showcase", 40, 10) {
        taskbarTitle = "Subwindows!";
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 1);
        vbox.Add<litw::Button>("Open Subwindow", [this]() {
            OpenSubwindow(wm, "Toggle Bool Subwindow", 35, 8, [this](litw::Subwindow& sub) {
                auto& subBox = sub.Add<litw::VerticalContainer>(2, 2, 1);
                subBox.Add<litw::Button>("Toggle Bool", [this]() {
                    SubwindowBool = !SubwindowBool;
                    });
                subBox.Add<litw::Button>("Close", [&sub]() { sub.CloseWindow(wm); });
                });
            });

        BoolSomething = &vbox.Add<litw::Label>("");
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
    void Draw(std::ostream& buffer) override {
        if (SubwindowBool) {
            BoolSomething->text = "Its true!";
        }
        else {
            BoolSomething->text = "Its (not)true!";
        }
        Window::Draw(buffer);
    }
};

int main() {
    auto startalert = litw::makeSharedWindow<StartAlert>();
    auto start = startmenu<litw::StartMenuWindow>(&wm);
    start->AddItem<README>("README");
    start->AddItem<MenuWindow>("Main Menu", "main menu");
    start->AddItem<NumberInputWindow>("Number Input Demo");
    start->AddItem<Textinputer>("Text Input Demo");
    start->AddItem<TextInputLargeDemo>("Text Input Demo - Large");
    start->AddItem<Users>("Table of Users");
    start->AddItem("Terminal", &ShellWindow::Create);
    start->AddItem<ContainerTest>("Containers");
    start->AddItem<EyesWindow>("Eyes");
    start->AddItem<PaletteTest>("Palette Test");
    start->AddItem<WindowWithSubwindow>("Subwindow showcase");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.AddWindow(startalert);
    wm.Run();
    wm.exit(1);
    return 0;
}
