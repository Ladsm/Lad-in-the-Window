#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class HorizCon : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int spacing = 2;
    int internalFocus = -1;
    HorizCon(int x, int y) {
        this->x = x;
        this->y = y;
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
    template<typename T, typename... Args>
    T& Add(Args&&... args) {
        auto widget = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *widget;
        AddWidget(std::move(widget));
        return ref;
    }
    void Layout() {
        int offsetX = 0;
        for (size_t i = 0; i < children.size(); i++) {
            auto& child = children[i];
            child->x = offsetX;
            child->y = 0;
            int w = child->GetWidth();
            if (w <= 0) w = 10;
            offsetX += w + spacing;
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
        if (input == InputType::MoveRight) {
            int start = internalFocus;
            do {
                internalFocus = (internalFocus + 1) % children.size();
            } while (!children[internalFocus]->focusable && internalFocus != start);
            return;
        }
        if (input == InputType::MoveLeft) {
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
        int total = 0;
        for (const auto& c : children) {
            total += c->GetWidth();
        }
        if (!children.empty())
            total += spacing * (children.size() - 1);
        return total;
    }
    int GetHeight() const override {
        int maxH = 0;
        for (const auto& c : children) {
            maxH = std::max(maxH, c->GetHeight());
        }
        return maxH;
    }
};