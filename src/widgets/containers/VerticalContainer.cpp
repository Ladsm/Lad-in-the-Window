#include <LITW/widgets/containers/VerticalContainer.hpp>
#include <LITW/widgets/Separator.hpp>
#include <algorithm>

namespace litw {
    
    VerticalContainer::VerticalContainer(int x, int y) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer::VerticalContainer(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->spacing = z;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer::VerticalContainer() {
        this->x = 0;
        this->y = 0;
        this->focusable = true;
        this->IsContainer = true;
    }
    VerticalContainer::VerticalContainer(int z) {
        this->x = 0;
        this->y = 0;
        this->spacing = z;
        this->focusable = true;
        this->IsContainer = true;
    }
    
    void VerticalContainer::EnsureValidFocus() {
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
    
    Widget* VerticalContainer::GetActiveWidget() {
        if (internalFocus >= 0 && internalFocus < static_cast<int>(children.size())) {
            return children[internalFocus]->GetActiveWidget();
        }
        return this;
    }
    
    void VerticalContainer::AddWidget(std::unique_ptr<Widget> w) {
        w->parent = this->parent ? this->parent : nullptr;
        children.push_back(std::move(w));
    }
    
    void VerticalContainer::Layout() {
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
    
    bool VerticalContainer::HandleMouseClick(int mx, int my, int px, int py) {
        Layout();
        int containerPx = px + x;
        int containerPy = py + y;
    
        for (size_t i = 0; i < children.size(); ++i) {
            if (i >= children.size()) break;
    
            int childPx = children[i]->isSeparator ? px : containerPx;
            int childPy = containerPy;
    
            if (children[i]->HandleMouseClick(mx, my, childPx, childPy)) {
                if (i < children.size() && (children[i]->focusable || children[i]->IsContainer)) {
                    internalFocus = static_cast<int>(i);
                }
                return true;
            }
        }
        return false;
    }
    
    void VerticalContainer::Draw(std::ostream& buffer, int px, int py) {
        Layout();
        EnsureValidFocus();
    
        for (size_t i = 0; i < children.size(); i++) {
            children[i]->focused = (this->focused && (int)i == internalFocus);
            if (children[i]->isSeparator) {
                children[i]->Draw(buffer, px, py + y);
            }
            else {
                children[i]->Draw(buffer, px + x, py + y);
            }
        }
    }
    
    void VerticalContainer::HandleInput(InputType input) {
        if (children.empty()) return;
        EnsureValidFocus();
    
        if (input == InputType::ArrowDown) {
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
    
        if (input == InputType::ArrowUp) {
            int start = (internalFocus < 0) ? 0 : internalFocus;
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
    
    int VerticalContainer::GetWidth() const {
        int maxW = 0;
        for (const auto& c : children) {
            maxW = std::max(maxW, c->GetWidth());
        }
        return maxW;
    }
    
    int VerticalContainer::GetHeight() const {
        int total = 0;
        for (const auto& c : children) {
            total += c->GetHeight();
        }
        if (!children.empty())
            total += spacing * (children.size() - 1);
        return total;
    }
}