#include <widgets/text/ScrollableTextBox.hpp>
#include <Window.hpp>

namespace litw {
    
    ScrollableTextBox::ScrollableTextBox(int x, int y, int h, int w, const std::vector<std::string>& t) {
        this->x = x;
        this->y = y;
        this->amountOfLinesToDraw = h;
        this->width = w;
        this->text = t;
        this->focusable = true;
    }
    ScrollableTextBox::ScrollableTextBox(int h, int w, const std::vector<std::string>& t) {
        this->x = 0;
        this->y = 0;
        this->amountOfLinesToDraw = h;
        this->width = w;
        this->text = t;
        this->focusable = true;
    }
    int ScrollableTextBox::GetWidth() const {
        return width;
    }
    int ScrollableTextBox::GetHeight() const {
        return amountOfLinesToDraw;
    }
    void ScrollableTextBox::Draw(std::ostream& buffer, int px, int py) {
        std::string bg = parent->Palette.Body;
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
    void ScrollableTextBox::HandleInput(InputType input) {
        if (input == InputType::Space && scroll + amountOfLinesToDraw < text.size()) {
            scroll++;
        }
        else if (input == InputType::Tab && scroll > 0) {
            scroll--;
        }
    }

}