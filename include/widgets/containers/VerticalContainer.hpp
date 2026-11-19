#pragma once
#include <Widget.hpp>
#include <vector>
#include <memory>

namespace litw {

    class VerticalContainer : public Widget {
    public:
        std::vector<std::unique_ptr<Widget>> children;
        int spacing = 0;
        int internalFocus = 0;
        VerticalContainer(int x, int y);
        VerticalContainer(int x, int y, int z);
        VerticalContainer();
        VerticalContainer(int z);
        void EnsureValidFocus();
        Widget* GetActiveWidget();
        void AddWidget(std::unique_ptr<Widget> w);
        template<typename T, typename... Args>
        T& Add(Args&&... args) {
            auto widget = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *widget;
            AddWidget(std::move(widget));
            return ref;
        }
        void Layout();
        bool HandleMouseClick(int mx, int my, int px, int py);
        void Draw(std::ostream& buffer, int px, int py);
        void HandleInput(InputType input);
        int GetWidth() const override;
        int GetHeight() const override;
    };

}