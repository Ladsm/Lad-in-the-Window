#include <LITW.hpp>
#include <memory>
WindowManager wm;
class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 10) {
        auto& vbox = Add<VertCon>(2, 2, 2);
        vbox.Add<Label>(0, 0, msg);
        vbox.Add<Button>(0, 0, "Close", [this]() { wm.RemoveWindow(this); });
    }
};
int main() {
    wm.AddWindow(std::make_shared<TextWindow>("Hello World", "Hello World"));
    wm.Run();
    wm.exit(1);
    return 0;
}