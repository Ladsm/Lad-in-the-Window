#include "getwh.hpp"
#include "Window.hpp"
#include "init.hpp"
#include <iostream>

class MenuWindow : public Window {
public:
    int selection = 0;
    MenuWindow() : Window("Main Menu", 30, 10) {}

    void Draw(std::ostream& buffer) override {
        Window::Draw(buffer);
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        std::string items[] = { "Start Game", "Options", "Exit" };
        for (int i = 0; i < 3; i++) {
            buffer << "\033[" << (y + 5 + i) << ";" << (x + 3) << "H";
            buffer << (selection == i ? "-> " : "   ") << items[i];
        }
        buffer << "\033[0m";
    }
    void HandleInput(InputType input) override {
        if (input == InputType::MoveUp) selection = (selection > 0) ? selection - 1 : 2;
        if (input == InputType::MoveDown) selection = (selection < 2) ? selection + 1 : 0;
        if (input == InputType::Enter) {
            switch (selection) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                std::exit(0);
                break;
            }
        }
    }
};
class TextWindow : public Window {
public:
    std::string message;
    TextWindow(std::string title, std::string msg) : Window(title, 40, 10), message(msg) {}
    TextWindow(std::string title, std::string msg, int w, int h) : Window(title, w, h), message(msg) {}
    void Draw(std::ostream& buffer) override {
        Window::Draw(buffer);
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        buffer << "\033[" << (y + 5) << ";" << (x + 3) << "H";
        buffer << message;
        buffer << "\033[" << (y + 8) << ";" << (x + 3) << "H";
        buffer << "[ Press Enter to Close ]";
        buffer << "\033[0m";
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Enter) {
            this->visible = false;
        }
    }
};

int main() {
    init();
    WindowManager wm;
    MenuWindow menu;
    TextWindow status("System status", "System is running smoothly!");
    TextWindow malware("wana decript", "Oh no! your files have been encripted! pay me", 60, 10);
    wm.AddWindow(&menu);
    wm.AddWindow(&status);
    wm.AddWindow(&malware);
    wm.Run();
    return 0;
}