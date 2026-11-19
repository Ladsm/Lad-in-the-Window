#include <LITW.hpp>

litw::WindowManager wm;

class TextWindow : public litw::Window {
    std::string text = "Lorem Ipsum";
    int number = 0;
public:
    TextWindow(std::string title, std::string msg)
        : litw::Window(title, 40, 12) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 1);
        vbox.Add<litw::Label>(msg);
        vbox.Add<litw::TextInput>(20, &text);
        vbox.Add<litw::NumberInput<int>>(3, &number);
        vbox.Add<litw::Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

int main() {
    auto start = startmenu<litw::StartMenuWindow>(&wm);
    start->AddItem<TextWindow>("Hello world", "Hello world", "Hello world");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}