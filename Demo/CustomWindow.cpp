#include <LITW.hpp>
#include <memory>
WindowManager wm;
class TextWindow : public Window {
public:
    TextWindow(std::string title, std::string msg)
        : Window(title, 40, 10) {
        AddWidget(std::make_unique <Label>(2, 2, msg));
        AddWidget(std::make_unique <Button>(2, 5, "Close", [this]() {
            wm.RemoveWindow(this);
        }));
    }
};
int main() {
    wm.AddWindow(std::make_shared<TextWindow>("Hello World", "Hello World"));
    wm.Run();
    wm.exit(1);
    return 0;
}