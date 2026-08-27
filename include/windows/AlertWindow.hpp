#pragma once
#include "Window.hpp"
#include "widgets/text/Label.hpp" 
#include "widgets/Button.hpp"
#include "widgets/containers/VerticalContainer.hpp"

class AlertWindow : public Window {
public:
    AlertWindow(std::string msg, WindowManager* wm) : Window("Alert", 40, 9) {
        if (msg.size() > 35) {
            startWidth = msg.size() + 4;
            width = startWidth;
        }
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<Label>(msg);
        vbox.Add<Button>("  OK  ", [wm, this]() {
            wm->RemoveWindow(this);
        });
    }
};