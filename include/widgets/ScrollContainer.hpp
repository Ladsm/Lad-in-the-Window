#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>

class ScrollContainer : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int internalFocus = 0;
    int scroll = 0;
    int viewWidth, viewHeight;
    ScrollContainer(int x, int y, int w, int h) {
        this->x = x; this->y = y;
        this->viewWidth = w; this->viewHeight = h;
        this->focusable = true;
    }
    void AddWidget(std::unique_ptr<Widget> w) {
        children.push_back(std::move(w));
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string bg = "\033[38;2;0;0;0;48;2;192;192;192m";
        for (int i = 0; i < viewHeight; ++i) {
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H" << bg << std::string(viewWidth, ' ');
        }
        for (int i = 0; i < (int)children.size(); i++) {
            auto& child = children[i];
            child->focused = (this->focused && internalFocus == i);
            int relativeY = child->y - scroll;
            if (relativeY >= 0 && relativeY < viewHeight) {
                child->Draw(buffer, px + x, py + y - scroll);
            }
        }
        int maxContentY = 0;
        for (const auto& child : children) {
            if (child->y + 1 > maxContentY) maxContentY = child->y + 1;
        }
        if (maxContentY > viewHeight) {
            int scrollbarX = px + x + viewWidth - 1;
            std::string barBg = "\033[38;2;100;100;100;48;2;192;192;192m";
            for (int i = 0; i < viewHeight; i++) {
                int thumbPos = (scroll * (viewHeight - 1)) / (maxContentY - viewHeight);

                buffer << "\033[" << (py + y + i) << ";" << scrollbarX << "H" << barBg;

                if (i == thumbPos) {
                    buffer << "█";
                }
                else {
                    buffer << "│";
                }
            }
        }
    }
    void HandleInput(InputType input) override {
        if (children.empty()) return;
        int oldFocus = internalFocus;
        if (input == InputType::Space) {
            if (internalFocus < (int)children.size() - 1) internalFocus++;
        }
        else if (input == InputType::Tab) {
            if (internalFocus > 0) internalFocus--;
        }
        else {
            children[internalFocus]->HandleInput(input);
            return;
        }
        if (internalFocus != oldFocus) {
            auto& target = children[internalFocus];
            if (target->y - scroll >= viewHeight) {
                scroll = target->y - viewHeight + 1;
            }
            else if (target->y - scroll < 0) {
                scroll = target->y;
            }
        }
    }
};