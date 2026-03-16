#pragma once
#include "Widget.hpp"
#include "Window.hpp"
#include <vector>
#include <string>
#include <memory>
class ShellWidget : public Widget {
private:
    std::vector<std::string> buffer;
    std::string currentInput;
    std::weak_ptr<Window> parentWindow;
public:
    ShellWidget(int px, int py, int w, int h, std::weak_ptr<Window> parent);
    void Draw(std::ostream& bufferStream, int parentX, int parentY) override;
    bool WantsRawInput() override { return true; }
    void HandleRawInput() override;
    void ExecuteCommand(const std::string& cmd);
};