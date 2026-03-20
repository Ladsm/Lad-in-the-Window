#pragma once
#include <Window.hpp>
#include <widgets/Button.hpp>
#include <userinput.hpp>
#include <vector>
#include <functional>
#include <memory>

class WindowManager;
class StartMenuWindow : public Window {
public:
    std::vector<std::function<std::shared_ptr<Window>()>> factories;
    WindowManager* wm;
    StartMenuWindow(WindowManager* manager)
        : Window("Start", 30, 10), wm(manager)
    {
        visible = false;
    }
    void AddItem(const std::string& name, std::function<std::shared_ptr<Window>()> factory) {
        size_t index = widgets.size();
        factories.push_back(factory);
        AddWidget(std::make_unique<Button>(
            2,
            1 + index,
            name,
            [this, index]() {
                auto w = factories[index]();
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