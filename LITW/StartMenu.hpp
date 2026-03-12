#pragma once
#include "Window.hpp"
#include "Button.hpp"
#include <vector>
#include <functional>

class WindowManager;

class StartMenuWindow : public Window {
public:
    std::vector<std::function<Window* ()>> factories;
    WindowManager* wm;
    StartMenuWindow(WindowManager* manager)
        : Window("Start", 30, 10), wm(manager)
    {
        visible = false;
    }
    void AddItem(const std::string& name, std::function<Window* ()> factory) {
        int index = widgets.size();
        factories.push_back(factory);
        AddWidget(new Button(
            2,
            1 + index,
            name,
            [this, index]() {
                Window* w = factories[index]();
                wm->AddWindow(w);
                visible = false;
            }
        ));
    }
    void Draw(std::ostream& buffer) {
        x = getConsoleWidth() - width;
        y = getConsoleHeight() - height - 1;

        Window::Draw(buffer);
    }
    void HandleInput(InputType input) {
        if (input == InputType::Escape) {
            visible = false;
            return;
        }
        Window::HandleInput(input);
    }
};