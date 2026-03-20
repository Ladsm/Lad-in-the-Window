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
    void HighlightLine(std::ostream& buffer, const std::string& line, int width, const std::string& bg) {
        std::string typeColor = "\033[38;2;86;156;214m";
        std::string funcColor = "\033[38;2;220;220;170m";
        std::vector<std::string> keywords = {
            "int", "void", "bool", "char", "double", "float", "class",
            "public", "override", "if", "else", "return", "static", "enum"
        };
        for (int col = 0; col < width; ++col) {
            bool highlighted = false;
            if (col < (int)line.size()) {
                for (const auto& kw : keywords) {
                    size_t kwLen = kw.size();
                    if (col + kwLen <= line.size() && line.compare(col, kwLen, kw) == 0) {
                        bool isStart = (col == 0 || !isalnum((unsigned char)line[col - 1]) && line[col - 1] != '_');
                        bool isEnd = (col + kwLen == line.size() || !isalnum((unsigned char)line[col + kwLen]) && line[col + kwLen] != '_');
                        if (isStart && isEnd) {
                            buffer << typeColor << kw << bg;
                            col += (int)kwLen - 1;
                            highlighted = true;
                            break;
                        }
                    }
                }
                if (!highlighted && isalpha((unsigned char)line[col])) {
                    int end = col;
                    while (end < (int)line.size() && (isalnum((unsigned char)line[end]) || line[end] == '_')) {
                        end++;
                    }
                    if (end < (int)line.size() && line[end] == '(') {
                        std::string funcName = line.substr(col, end - col);
                        buffer << funcColor << funcName << bg;
                        col = end - 1;
                        highlighted = true;
                    }
                }
            }
            if (!highlighted) {
                buffer << (col < (int)line.size() ? line[col] : ' ');
            }
        }
    }
    bool WantsRawInput() override { return focused; }
    void HandleRawInput() override {
        if (!lines || lines->empty()) return;
        cursorY = std::max(0, std::min(cursorY, (int)lines->size() - 1));
        cursorX = std::max(0, std::min(cursorX, (int)(*lines)[cursorY].size()));
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
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H" << bg;
            if (lineIndex < (int)lines->size()) {
                HighlightLine(buffer, (*lines)[lineIndex], width, bg);
                if (focused && lineIndex == cursorY && cursorX < width) {
                    char ch = (cursorX < (int)(*lines)[lineIndex].size()) ? (*lines)[lineIndex][cursorX] : ' ';
                    buffer << "\033[" << (py + y + i) << ";" << (px + x + cursorX) << "H";
                    buffer << "\033[38;2;255;255;255;48;2;0;0;0m" << ch << bg;
                }
            }
            else {
                buffer << std::string(width, ' ');
            }
        }
        buffer << "\033[" << (py + y + height - 1) << ";" << (px + x) << "H\033[7m "
            << (mode == INSERT ? "-- INSERT --" : "-- COMMAND --") << " \033[0m";
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
