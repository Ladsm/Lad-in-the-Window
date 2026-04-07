#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class VertCon : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int spacing = 0;
    int internalFocus = -1;
    VertCon(int x, int y) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        this->IsContainer = true;
    }
    VertCon(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->spacing = z;
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
        int offsetY = 0;
        for (size_t i = 0; i < children.size(); i++) {
            auto& child = children[i];
            child->x = 0;
            child->y = offsetY;
            int h = child->GetHeight();
            if (h <= 0) h = 1;
            offsetY += h;
            if (i < children.size() - 1) {
                offsetY += spacing;
            }
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
        if (input == InputType::MoveDown) {
            if (internalFocus < 0) {
                for (size_t i = 0; i < children.size(); i++) {
                    if (children[i]->focusable) {
                        internalFocus = (int)i;
                        return;
                    }
                }
                return;
            }
            for (int i = internalFocus + 1; i < (int)children.size(); i++) {
                if (children[i]->focusable) {
                    internalFocus = i;
                    return;
                }
            }
            internalFocus = -1;
            return;
        }
        if (input == InputType::MoveUp) {
            if (internalFocus < 0) {
                for (int i = (int)children.size() - 1; i >= 0; i--) {
                    if (children[i]->focusable) {
                        internalFocus = i;
                        return;
                    }
                }
                return;
            }
            for (int i = internalFocus - 1; i >= 0; i--) {
                if (children[i]->focusable) {
                    internalFocus = i;
                    return;
                }
            }
            internalFocus = -1;
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