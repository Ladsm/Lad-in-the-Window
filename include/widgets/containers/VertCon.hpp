#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class VertCon : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int spacing = 1;
    int internalFocus = -1;
    VertCon(int x, int y) {
        this->x = x;
        this->y = y;
        this->focusable = true;
    }
    void AddWidget(std::unique_ptr<Widget> w) {
        children.push_back(std::move(w));
        if (internalFocus == -1 && children.back()->focusable) {
            internalFocus = (int)children.size() - 1;
        }
    }
    void Layout() {
        int offsetY = 0;
        for (size_t i = 0; i < children.size(); i++) {
            auto& child = children[i];
            child->x = 0;
            child->y = offsetY;
            int h = child->GetHeight();
            if (h <= 0) h = 1;
            offsetY += h + spacing;
        }
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        Layout();
        for (size_t i = 0; i < children.size(); i++) {
            auto& child = children[i];

            child->focused = (this->focused && (int)i == internalFocus);

            child->Draw(buffer, px + x, py + y);
        }
    }
    void HandleInput(InputType input) override {
        if (children.empty()) return;
        if (input == InputType::Space) {
            int start = internalFocus;
            do {
                internalFocus = (internalFocus + 1) % children.size();
            } while (!children[internalFocus]->focusable && internalFocus != start);
            return;
        }
        if (input == InputType::Tab) {
            int start = internalFocus;
            do {
                internalFocus--;
                if (internalFocus < 0)
                    internalFocus = (int)children.size() - 1;
            } while (!children[internalFocus]->focusable && internalFocus != start);
            return;
        }
        if (internalFocus >= 0 && internalFocus < (int)children.size()) {
            children[internalFocus]->HandleInput(input);
        }
    }
    int GetWidth() const override {
        int maxW = 0;
        for (const auto& c : children) {
            maxW = std::max(maxW, c->GetWidth());
        }
        return maxW;
    }
    int GetHeight() const override {
        int total = 0;
        for (const auto& c : children) {
            total += c->GetHeight();
        }
        if (!children.empty())
            total += spacing * (children.size() - 1);
        return total;
    }
};