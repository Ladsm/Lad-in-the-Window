#pragma once
#include <LITW/Window.hpp>
#include <LITW/widgets/text/Label.hpp>
#include <LITW/widgets/Button.hpp>
#include <LITW/widgets/containers/VerticalContainer.hpp>

namespace litw {

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

}