#include <LITW/LITW.hpp>

litw::WindowManager wm;
class TextWindow : public litw::Window {
public:
    TextWindow(std::string title, std::string msg)
        : litw::Window(title, 40, 10) {
        auto& vbox = Add<litw::VerticalContainer>(2, 2, 2);
        vbox.Add<litw::Label>(msg);
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