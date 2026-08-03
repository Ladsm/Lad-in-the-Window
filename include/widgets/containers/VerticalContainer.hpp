#pragma once
#include "Widget.hpp"
#include <widgets/Separator.hpp>
#include <vector>
#include <memory>
#include <algorithm>

class VerticalContainer : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int spacing = 0;
    int internalFocus = 0;
    VerticalContainer(int x, int y) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->spacing = z;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer() {
        this->x = 0;
        this->y = 0;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer(int z) {
        this->x = 0;
        this->y = 0;
        this->spacing = z;
        this->focusable = true;
        this->IsContainer = true;
    }
    void EnsureValidFocus() {
        if (internalFocus >= 0 &&
            internalFocus < static_cast<int>(children.size()) &&
            children[internalFocus]->focusable) {
            return;
        }
        for (size_t i = 0; i < children.size(); i++) {
            if (children[i]->focusable) {
                internalFocus = static_cast<int>(i);
                return;
            }
        }
        internalFocus = -1;
    }
    Widget* GetActiveWidget() override {
        if (internalFocus >= 0 && internalFocus < children.size()) {
            return children[internalFocus]->GetActiveWidget();
        }
        return this;
    }
    void AddWidget(std::unique_ptr<Widget> w) {
        w->parent = this->parent ? this->parent : nullptr;
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
        if (this->focused && (internalFocus < 0 || !children[internalFocus]->focusable)) {
            EnsureValidFocus();
        }
        for (size_t i = 0; i < children.size(); i++) {
            auto& child = children[i];
            child->focused = (this->focused && (int)i == internalFocus);
            if(child->isSeparator){
                child->Draw(buffer, px, py + y);
            }
            else {
                child->Draw(buffer, px + x, py + y);
            }
        }
    }
    void HandleInput(InputType input) override {
        if (children.empty()) return;
        EnsureValidFocus();
        if (input == InputType::MoveDown) {
            int start = (internalFocus < 0) ? 0 : internalFocus;
            int next = start;
            do {
                next = (next + 1) % children.size();
                if (children[next]->focusable) {
                    internalFocus = next;
                    return;
                }
            } while (next != start);
            return;
        }

        if (input == InputType::MoveUp) {
            int start = (internalFocus < 0) ? 0 : internalFocus;
            int next = start;
            do {
                next = (next - 1 + children.size()) % children.size();
                if (children[next]->focusable) {
                    internalFocus = next;
                    return;
                }
            } while (next != start);
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