#pragma once
#include <Widget.hpp>
#include <Window.hpp>
#include <userinput.hpp>
#include <vector>
#include <string>
#include <algorithm>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
class LargeTextInput : public Widget {
public:
    enum Mode { COMMAND, INSERT };
    std::vector<std::string>* lines;
    int cursorX = 0;
    int cursorY = 0;
    int scroll = 0;
    int height;
    int width;
    Mode mode = COMMAND;
    Window* win;
    LargeTextInput(int x, int y, int h, int w, std::vector<std::string>* target, Window* pWin)
        : lines(target), height(h), width(w), win(pWin) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        if (lines && lines->empty()) {
            lines->push_back("");
        }
    }
    bool WantsRawInput() override {
        return focused;
    }
    void HandleRawInput() override {
        if (!lines || lines->empty()) return;
        cursorY = std::max(0, std::min(cursorY, (int)lines->size() - 1));
        cursorX = std::max(0, std::min(cursorX, (int)(*lines)[cursorY].size()));
        if (!lines || lines->empty()) return;
        int key = readKey();
        if (mode == COMMAND) {
            if (key == '\r' || key == '\n') {
                this->focused = false;
                return;
            }
            switch (key) {
            case 'i': mode = INSERT; return;
            case 'h': if (cursorX > 0) cursorX--; return;
            case 'l': if (cursorX < (int)(*lines)[cursorY].size()) cursorX++; return;
            case 'k': if (cursorY > 0) cursorY--; return;
            case 'j': if (cursorY < (int)lines->size() - 1) cursorY++; return;
            }
        }
        else if (mode == INSERT) {
            if (key == 27) {
                mode = COMMAND;
                return;
            }
            if (key == 8 || key == 127) {
                if (cursorX > 0) {
                    (*lines)[cursorY].erase(cursorX - 1, 1);
                    cursorX--;
                }
                else if (cursorY > 0) {
                    cursorX = (*lines)[cursorY - 1].size();
                    (*lines)[cursorY - 1] += (*lines)[cursorY];
                    lines->erase(lines->begin() + cursorY);
                    cursorY--;
                }
                return;
            }
            if (key == '\n' || key == '\r') {
                std::string remainder = (*lines)[cursorY].substr(cursorX);
                (*lines)[cursorY] = (*lines)[cursorY].substr(0, cursorX);
                lines->insert(lines->begin() + cursorY + 1, remainder);
                cursorY++;
                cursorX = 0;
                return;
            }
            if (key >= 32 && key <= 126) {
                (*lines)[cursorY].insert(cursorX, 1, (char)key);
                cursorX++;
            }
        }
        cursorY = std::max(0, std::min(cursorY, (int)lines->size() - 1));
        cursorX = std::max(0, std::min(cursorX, (int)(*lines)[cursorY].size()));
        if (cursorY < scroll) scroll = cursorY;
        if (cursorY >= scroll + height) scroll = cursorY - height + 1;

    }
    void Draw(std::ostream& buffer, int px, int py) override {
        if (!lines) return;
        std::string bg = "\033[38;2;0;0;0;48;2;192;192;192m";
        for (int i = 0; i < height; ++i) {
            int lineIndex = scroll + i;
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H";
            buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
            std::string empty = "";
            std::string& line =
                (lineIndex < (int)lines->size())
                ? (*lines)[lineIndex]
                : empty;
            for (int col = 0; col < width; ++col) {
                char ch = (col < (int)line.size()) ? line[col] : ' ';
                bool isCursor =
                    focused &&
                    (lineIndex == cursorY) &&
                    (col == cursorX || (cursorX == (int)line.size() && col == cursorX));
                if (isCursor) {
                    buffer << "\033[38;2;255;255;255;48;2;0;0;0m" << (col < (int)line.size() ? line[col] : ' ') << bg;
                }
                else {
                    buffer << (col < (int)line.size() ? line[col] : ' ');
                }
            }
        }
        buffer << "\033[" << (py + y + height - 1) << ";" << (px + x) << "H";
        buffer << "\033[7m ";
        buffer << (mode == INSERT ? "-- INSERT --" : "-- COMMAND --");
        buffer << " \033[0m";
        if (focused) {
            int drawY = cursorY - scroll;
            if (drawY >= 0 && drawY < height) {
                buffer << "\033["
                    << (py + y + drawY) << ";"
                    << (px + x + cursorX) << "H";
                buffer << "\033[?25h";
            }
        }
        else {
            buffer << "\033[?25l";
        }
    }
    void HandleInput(InputType input) override {
        if (!focused) return;
        if (mode == COMMAND && input == InputType::Enter) {
            focused = false;
            if (win) {
                win->focusedWidget = std::min((int)win->widgets.size() - 1, win->focusedWidget + 1);
            }
        }
    }
};