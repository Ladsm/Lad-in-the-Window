#pragma once
#include "Window.hpp"
#include "widgets/text/Label.hpp" 
#include "widgets/Button.hpp"

class AlertWindow : public Window {
public:
    AlertWindow(std::string msg, WindowManager* wm) : Window("Alert", 40, 8) {
        AddWidget(std::make_unique<CenteredLabel>(2, msg, this));
        AddWidget(std::make_unique<Button>(15, 5, "  OK  ", [wm, this]() {
            wm->RemoveWindow(this);
            }));
        focusedWidget = 1;
    }
};