#pragma once

#include <Window.hpp>
#include <widgets/Button.hpp>
#include <userinput.hpp>

#include <memory>
#include <functional>
#include <string>
#include <tuple>
#include <utility>

class WindowManager;

class StartMenuWindow : public Window {
public:
    explicit StartMenuWindow(WindowManager* manager)
        : Window("Start", 30, 10), wm(manager) {
        visible = false;
    }
    template<typename T, typename... Args>
    void AddItem(const std::string& name, Args&&... args) {
        AddItem(name,
            [argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                return std::apply(
                    [](auto&&... unpacked) {
                        return std::make_shared<T>(
                            std::forward<decltype(unpacked)>(unpacked)...
                        );
                    },
                    std::move(argsTuple)
                );
            }
        );
    }
    void AddItem(const std::string& name,
        std::function<std::shared_ptr<Window>()> factory)
    {
        AddWidget(std::make_unique<Button>(
            2,
            1 + widgets.size(),
            name,
            [this, factory]() {
                wm->AddWindow(factory());
                visible = false;
            }
        ));
    }
    void Draw(std::ostream& buffer) override {
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

private:
    WindowManager* wm;
};