#pragma once
#include "Window.hpp"
#include <vector>
#include <string>
#include <functional>

class WindowManager;

class StartMenuWindow : public Window {
public:
    std::vector<std::function<Window* ()>> factories;
    std::vector<std::string> labels;

    int selection = 0;
    WindowManager* wm;

    StartMenuWindow(WindowManager* manager)
        : Window("Start", 30, 10), wm(manager)
    {
        visible = false;
    }

    void AddItem(const std::string& name, std::function<Window* ()> factory) {
        labels.push_back(name);
        factories.push_back(factory);
    }

    void Draw(std::ostream& buffer) override {

        // anchor bottom-right
        x = getConsoleWidth() - width;
        y = getConsoleHeight() - height - 1;

        Window::Draw(buffer);

        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";

        for (size_t i = 0; i < labels.size(); i++) {
            buffer << "\033[" << (y + 4 + i) << ";" << (x + 3) << "H";
            buffer << (selection == i ? "-> " : "   ") << labels[i];
        }

        buffer << "\033[0m";
    }

    void HandleInput(InputType input) override {

        if (input == InputType::MoveUp)
            selection = (selection > 0) ? selection - 1 : labels.size() - 1;

        if (input == InputType::MoveDown)
            selection = (selection < (int)labels.size() - 1) ? selection + 1 : 0;

        if (input == InputType::Enter) {
            Window* w = factories[selection]();
            wm->AddWindow(w);
            visible = false;
        }

        if (input == InputType::Escape) {
            visible = false;
        }
    }
};