#pragma once
#include "Window.hpp"
#include "widgets/text/Label.hpp"
#include "widgets/Button.hpp"
#include <memory>

class AlertWindow : public Window {
public:
    AlertWindow(std::string msg, WindowManager* wm) : Window("Alert", 40, 8) {
        AddWidget(std::make_unique<Label>(2, 2, msg));
        AddWidget(std::make_unique<Button>(15, 5, "  OK  ", [this, wm]() {
            wm->RemoveWindow(this);
            }));
        focusedWidget = 1;
    }
};