#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class HorizontalContainer : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
    int spacing = 2;
    int internalFocus = 0;

    HorizontalContainer(int x, int y) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        this->IsContainer = true;
    }
    HorizontalContainer(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->spacing = z;
        this->focusable = true;
        this->IsContainer = true;
    }
    HorizontalContainer(int x) {
        this->x = 0;
        this->y = 0;
        this->spacing = x;
        this->focusable = true;
        this->IsContainer = true;
    }
    HorizontalContainer() {
        this->x = 0;
        this->y = 0;
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
        if (internalFocus >= 0 && internalFocus < static_cast<int>(children.size())) {
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

    bool HandleMouseClick(int mx, int my, int px, int py) override {
        Layout();
        int containerPx = px + x;
        int containerPy = py + y;

        for (size_t i = 0; i < children.size(); ++i) {
            if (i >= children.size()) break;

            if (children[i]->HandleMouseClick(mx, my, containerPx, containerPy)) {
                if (i < children.size() && (children[i]->focusable || children[i]->IsContainer)) {
                    internalFocus = static_cast<int>(i);
                }
                return true;
            }
        }
        return false;
    }

    void Draw(std::ostream& buffer, int px, int py) override {
        Layout();
        EnsureValidFocus();
        for (size_t i = 0; i < children.size(); i++) {
            children[i]->focused = (this->focused && (int)i == internalFocus);
            children[i]->Draw(buffer, px + x, py + y);
        }
    }

    void HandleInput(InputType input) override {
        if (children.empty()) return;
        EnsureValidFocus();
        if (input == InputType::MoveRight) {
            int start = internalFocus < 0 ? 0 : internalFocus;
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
        if (input == InputType::MoveLeft) {
            int start = internalFocus < 0 ? 0 : internalFocus;
            int next = start;
            do {
                next = (next - 1 + static_cast<int>(children.size())) % children.size();
                if (children[next]->focusable) {
                    internalFocus = next;
                    return;
                }
            } while (next != start);
            return;
        }
        if (internalFocus >= 0 && internalFocus < static_cast<int>(children.size())) {
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