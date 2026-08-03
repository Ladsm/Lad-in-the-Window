#include <LITW.hpp>

WindowManager wm;

class TextWindow : public Window {
    std::string text = "Lorem Ipsum";
    int number = 0;
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 12) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<Label>(msg);
        vbox.Add<TextInput>(20, &text);
        vbox.Add<NumberInput>(3, &number);
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

int main() {
    auto start = startmenu<StartMenuWindow>(&wm);
    start->AddItem<TextWindow>("Hello world", "Hello world", "Hello world");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}