#pragma once
#include <Widget.hpp>

namespace litw {

    class EyesWidget : public Widget {
    public:
        int eyeWidth;
        int eyeHeight;
        EyesWidget(int x, int y, int w, int h);
        EyesWidget(int w, int h);
        int GetWidth() const override;
        int GetHeight() const override;
        void Draw(std::ostream& buffer, int px, int py) override;
    private:
        void DrawSingleEye(std::ostream& buffer, int eX, int eY);
    };

}