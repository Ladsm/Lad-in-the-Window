#pragma once
#include "Widget.hpp"
#include <cmath>
#include <iostream>

class EyesWidget : public Widget {
public:
    int eyeWidth;
    int eyeHeight;
    EyesWidget(int x, int y, int w = 12, int h = 5) {
        this->x = x;
        this->y = y;
        this->eyeWidth = w;
        this->eyeHeight = h;
        this->focusable = false;
    }
    int GetWidth() const override { return eyeWidth; }
    int GetHeight() const override { return eyeHeight; }
    void Draw(std::ostream& buffer, int px, int py) override {
        int gap = 2;
        int individualEyeWidth = 6;
        int totalPairWidth = (individualEyeWidth * 2) + gap;
        int startX = px + x + (eyeWidth - totalPairWidth) / 2;
        int centerY = py + y + (eyeHeight / 2);
        DrawSingleEye(buffer, startX, centerY);
        DrawSingleEye(buffer, startX + individualEyeWidth + gap, centerY);
    }
private:
    void DrawSingleEye(std::ostream& buffer, int eX, int eY) {
        int mx = getMouseX();
        int my = getMouseY();
        int eyeCenterX = eX + 3;
        int eyeCenterY = eY;
        double dx = mx - eyeCenterX;
        double dy = (my - eyeCenterY) * 3.0;
        double dist = std::sqrt(dx * dx + dy * dy);
        int pupilX = 0;
        int pupilY = 0;
        if (dist > 0.5) {
            double maxRX = 1.8;
            double maxRY = 0.8;
            pupilX = (int)std::round((dx / dist) * maxRX);
            pupilY = (int)std::round((dy / dist) * maxRY);
        }
        std::string silverBg = "\033[38;2;255;255;255;48;2;192;192;192m";
        std::string whiteCell = "\033[48;2;255;255;255m";
        std::string blackPupil = "\033[38;2;0;0;0;48;2;255;255;255m";
        std::string reset = "\033[0m";
        buffer << "\033[" << (eY - 1) << ";" << (eX + 1) << "H" << silverBg << "▄▄▄▄" << reset;
        for (int i = 0; i <= 1; ++i) {
            buffer << "\033[" << (eY + i) << ";" << eX << "H" << silverBg << "█"
                << whiteCell << "    " << silverBg << "█" << reset;
        }
        buffer << "\033[" << (eY + 2) << ";" << (eX + 1) << "H" << silverBg << "▀▀▀▀" << reset;
        int drawPx = (eX + 2) + pupilX;
        int drawPy = eY + (pupilY > 0 ? 1 : 0);
        if (drawPx < eX + 1) drawPx = eX + 1;
        if (drawPx > eX + 4) drawPx = eX + 4;

        buffer << "\033[" << drawPy << ";" << drawPx << "H" << blackPupil << "●" << reset;
    }
};