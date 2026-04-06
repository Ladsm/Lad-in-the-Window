#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class ScrollContainer : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int internalFocus = 0;
    int scroll = 0;
    int viewWidth, viewHeight;
    ScrollContainer(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->viewWidth = w;
        this->viewHeight = h;
        this->focusable = true;
        this->IsContainer = true;
    }
    Widget* GetActiveWidget() override {
        if (internalFocus >= 0 && internalFocus < children.size()) {
            return children[internalFocus]->GetActiveWidget();
        }
        return this;
    }
    void AddWidget(std::unique_ptr<Widget> w) {
        children.push_back(std::move(w));
    }
    int GetContentHeight() const {
        int maxY = 0;
        for (const auto& c : children) {
            maxY = std::max(maxY, c->y + 1);
        }
        return maxY;
    }
    void ClampScroll() {
        int contentHeight = GetContentHeight();
        int maxScroll = std::max(0, contentHeight - viewHeight);
        scroll = std::clamp(scroll, 0, maxScroll);
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string bg = "\033[38;2;0;0;0;48;2;192;192;192m";
        int contentWidth = viewWidth - 1;
        for (int i = 0; i < viewHeight; ++i) {
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H"
                << bg << std::string(viewWidth, ' ');
        }
        for (int i = 0; i < (int)children.size(); i++) {
            auto& child = children[i];
            child->focused = (this->focused && internalFocus == i);
            int relativeY = child->y - scroll;
            if (relativeY < 0 || relativeY >= viewHeight)
                continue;
            child->Draw(buffer, px + x, py + y - scroll);
        }
        int contentHeight = GetContentHeight();
        if (contentHeight > viewHeight) {
            int scrollbarX = px + x + viewWidth - 1;
            int trackHeight = viewHeight;
            float ratio = (float)viewHeight / contentHeight;
            int thumbSize = std::max(1, (int)(trackHeight * ratio));
            int maxScroll = contentHeight - viewHeight;
            int thumbPos = (maxScroll > 0)
                ? (scroll * (trackHeight - thumbSize)) / maxScroll
                : 0;
            std::string barBg = "\033[38;2;100;100;100;48;2;192;192;192m";
            for (int i = 0; i < viewHeight; i++) {
                buffer << "\033[" << (py + y + i) << ";" << scrollbarX << "H" << barBg;
                if (i >= thumbPos && i < thumbPos + thumbSize)
                    buffer << "█";
                else
                    buffer << "│";
            }
        }
    }
    void HandleInput(InputType input) override {
        if (children.empty()) return;
        int oldFocus = internalFocus;
        if (input == InputType::Space || input == InputType::Tab) {
            int direction = (input == InputType::Space) ? 1 : -1;
            int start = internalFocus;
            do {
                internalFocus += direction;
                if (internalFocus < 0)
                    internalFocus = (int)children.size() - 1;
                if (internalFocus >= (int)children.size())
                    internalFocus = 0;
                if (children[internalFocus]->focusable)
                    break;
            } while (internalFocus != start);
        }
        else {
            if (internalFocus >= 0 && internalFocus < (int)children.size()) {
                children[internalFocus]->HandleInput(input);
            }
            return;
        }
        if (internalFocus != oldFocus) {
            auto& target = children[internalFocus];

            if (target->y < scroll) {
                scroll = target->y;
            }
            else if (target->y >= scroll + viewHeight) {
                scroll = target->y - viewHeight + 1;
            }
            ClampScroll();
        }
    }
};