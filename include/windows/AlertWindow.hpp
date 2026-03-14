#pragma once
#include "Window.hpp"
#include "widgets/Label.hpp"
#include "widgets/Button.hpp"
class AlertWindow : public Window {
public:
    AlertWindow(std::string msg, WindowManager* wm) : Window("Alert", 40, 8) {
        AddWidget(new Label(2, 2, msg));
        AddWidget(new Button(15, 5, "  OK  ", [this, wm]() {
            wm->RemoveWindow(this);
            }));
        focusedWidget = 1;
    }
};
