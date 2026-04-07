#pragma once
#include "Widget.hpp"
#include <vector>

class ScrollableTextBox : public Widget {
public:
    int scroll = 0;
    int amountOfLinesToDraw = 10;
    int width = 30;
    std::vector<std::string> text;
    ScrollableTextBox(int x, int y, int h, int w, const std::vector<std::string>& t) {
        this->x = x;
        this->y = y;
        this->amountOfLinesToDraw = h;
        this->width = w;
        this->text = t;
        this->focusable = true;
    }
    int GetWidth() const override {
        return width;
    }
    int GetHeight() const override {
        return amountOfLinesToDraw;
    }
    void Draw(std::ostream& buffer, int px, int py) override {
        std::string bg = "\033[38;2;0;0;0;48;2;192;192;192m";
        for (int i = 0; i < amountOfLinesToDraw; ++i) {
            int textIndex = i + scroll;
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H" << bg;
            std::string lineToPrint = "";
            if (textIndex < (int)text.size()) {
                lineToPrint = text[textIndex];
                if ((int)lineToPrint.length() > width - 3) {
                    lineToPrint = lineToPrint.substr(0, width - 3);
                }
            }
            buffer << lineToPrint;
            for (int s = 0; s < (width - 3 - (int)lineToPrint.length()); ++s) {
                buffer << " ";
            }
            buffer << "\033[38;2;255;255;255m";

            if (i == 0) {
                buffer << " ^ ";
            }
            else if (i == amountOfLinesToDraw - 1) {
                buffer << " v ";
            }
            else {
                int trackHeight = amountOfLinesToDraw - 2;
                int thumbPos = 1;
                if (text.size() > (size_t)amountOfLinesToDraw) {
                    float scrollPercent = (float)scroll / (text.size() - amountOfLinesToDraw);
                    thumbPos = 1 + (int)(scrollPercent * (trackHeight - 1));
                }
                if (i == thumbPos) {
                    buffer << " █ ";
                }
                else {
                    buffer << " │ ";
                }
            }
        }
        buffer << "\033[0m";
    }
    void HandleInput(InputType input) override {
        if (input == InputType::Space && scroll + amountOfLinesToDraw < text.size()) {
            scroll++;
        }
        else if (input == InputType::Tab && scroll > 0) {
            scroll--;
        }
    }
};