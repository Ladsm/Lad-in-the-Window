#include <LITW.hpp>

WindowManager wm;
class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 10) {
        auto& vbox = Add<VerticalContainer>(2, 2, 2);
        vbox.Add<Label>(msg);
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