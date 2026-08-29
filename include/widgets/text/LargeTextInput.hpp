#pragma once
#include <Widget.hpp>
#include <Window.hpp>
#include <userinput.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <widgets/containers/VerticalContainer.hpp>

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
    int scrollX = 0;
    int height;
    int width;
    bool highligh = false;
    bool isWriting = false;
    Mode mode = COMMAND;

    std::unordered_set<std::string> knownVariables;
    std::unordered_set<std::string> knownTypes = {
        "int", "void", "bool", "char", "double", "float", "long", "short",
        "auto", "uint8_t", "uint16_t", "uint32_t", "uint64_t", "int32_t",
        "int64_t", "string", "vector", "size_t"
    };

    LargeTextInput(int x, int y, int h, int w, std::vector<std::string>* target, bool hi)
        : lines(target), height(h), width(w), highligh(hi) {
        this->x = x;
        this->y = y;
        this->focusable = true;
        if (lines && lines->empty()) {
            lines->push_back("");
        }
    }

    LargeTextInput(int h, int w, std::vector<std::string>* target, bool hi)
        : lines(target), height(h), width(w), highligh(hi) {
        this->x = 0;
        this->y = 0;
        this->focusable = true;
        if (lines && lines->empty()) {
            lines->push_back("");
        }
    }

    int GetWidth() const override { return width; }
    int GetHeight() const override { return height; }

    void HighlightLine(std::ostream& buffer, const std::string& line, int width, const std::string& bg) {
        std::string kwColor = "\033[38;2;0;0;255m";
        std::string funcColor = "\033[38;2;130;130;0m";
        std::string strColor = "\033[38;2;180;0;0m";
        std::string numColor = "\033[38;2;0;100;0m";
        std::string preColor = "\033[38;2;120;0;120m";
        std::string opColor = "\033[38;2;0;130;130m";
        std::string varColor = "\033[38;2;220;120;40m";
        std::string typeColor = "\033[38;2;40;180;180m";

        std::vector<std::string> keywords = {
            "int", "void", "bool", "char", "double", "float", "long", "short", "signed", "unsigned",
            "const", "static", "volatile", "mutable", "auto", "nullptr", "constexpr", "inline",
            "class", "struct", "union", "enum", "public", "private", "protected", "virtual",
            "override", "final", "friend", "explicit", "this", "operator", "typename", "template",
            "if", "else", "switch", "case", "default", "return", "for", "while", "do",
            "break", "continue", "goto", "try", "catch", "throw", "noexcept",
            "and", "or", "not", "new", "delete", "sizeof", "decltype", "typeid",
            "static_cast", "dynamic_cast", "const_cast", "reinterpret_cast",
            "using", "namespace", "import", "export", "module", "std", "string", "vector"
        };

        std::string lastToken = "";
        bool expectingTypeName = false;

        for (int col = 0; col < width; ++col) {
            if (col >= (int)line.size()) {
                buffer << " ";
                continue;
            }
            bool highlighted = false;

            if (line[col] == '"' || line[col] == '\'') {
                char quote = line[col];
                int end = col + 1;
                while (end < (int)line.size() && line[end] != quote) {
                    if (line[end] == '\\' && end + 1 < (int)line.size()) end++;
                    end++;
                }
                if (end < (int)line.size()) end++;
                buffer << strColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (line[col] == '#') {
                int end = col;
                while (end < (int)line.size() && !isspace((unsigned char)line[end])) end++;
                buffer << preColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (col + 1 < (int)line.size()) {
                std::string duo = line.substr(col, 2);
                if (duo == "<<" || duo == ">>" || duo == "::" || duo == "->" || duo == "==" || duo == "!=") {
                    buffer << opColor << duo << bg;
                    col += 1;
                    highlighted = true;
                    lastToken = "";
                    expectingTypeName = false;
                }
            }

            if (!highlighted && std::string("+-*/%=!<>|&;{},()").find(line[col]) != std::string::npos) {
                buffer << opColor << line[col] << bg;
                highlighted = true;
                if (line[col] == ';' || line[col] == '{' || line[col] == '}') {
                    lastToken = "";
                    expectingTypeName = false;
                }
            }

            if (!highlighted && (isalpha((unsigned char)line[col]) || line[col] == '_')) {
                int start = col;
                int end = col;
                while (end < (int)line.size() && (isalnum((unsigned char)line[end]) || line[end] == '_')) {
                    end++;
                }
                std::string identifier = line.substr(start, end - start);

                bool isKw = false;
                for (const auto& kw : keywords) {
                    if (identifier == kw) {
                        isKw = true;
                        break;
                    }
                }
                if (expectingTypeName) {
                    knownTypes.insert(identifier);
                    buffer << typeColor << identifier << bg;
                    lastToken = identifier;
                    expectingTypeName = false;
                }
                else if (identifier == "struct" || identifier == "class" || identifier == "enum" || identifier == "typename") {
                    buffer << kwColor << identifier << bg;
                    expectingTypeName = true;
                    lastToken = identifier;
                }
                else if (isKw) {
                    buffer << kwColor << identifier << bg;
                    lastToken = identifier;
                }
                else if (end < (int)line.size() && line[end] == '(') {
                    buffer << funcColor << identifier << bg;
                    lastToken = "";
                }
                else if (knownTypes.find(identifier) != knownTypes.end()) {
                    buffer << typeColor << identifier << bg;
                    lastToken = identifier;
                }
                else {
                    if (knownTypes.find(lastToken) != knownTypes.end()) {
                        knownVariables.insert(identifier);
                        lastToken = "";
                    }

                    if (knownVariables.find(identifier) != knownVariables.end()) {
                        buffer << varColor << identifier << bg;
                    }
                    else {
                        buffer << identifier;
                    }
                }

                col = end - 1;
                highlighted = true;
            }
            else if (!highlighted && isdigit((unsigned char)line[col]) && (col == 0 || !isalnum((unsigned char)line[col - 1]))) {
                int end = col;
                while (end < (int)line.size() && isdigit((unsigned char)line[end])) end++;
                buffer << numColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            if (!highlighted) {
                buffer << line[col];
            }
        }
    }

    bool WantsRawInput() override { return isWriting; }

    void HandleRawInput() override {
        if (!lines || lines->empty()) return;

        cursorY = std::max(0, std::min(cursorY, (int)lines->size() - 1));
        cursorX = std::max(0, std::min(cursorX, (int)(*lines)[cursorY].size()));

        int key = readKey();
        bool isUp = (key == 1072 || key == 1065);
        bool isDown = (key == 1080 || key == 1066);
        bool isRight = (key == 1077 || key == 1067);
        bool isLeft = (key == 1075 || key == 1068);

        if (isLeft) {
            if (cursorX > 0) {
                cursorX--;
            }
            else if (cursorY > 0) {
                cursorY--;
                cursorX = (*lines)[cursorY].size();
            }
            return;
        }
        if (isRight) {
            if (cursorX < (int)(*lines)[cursorY].size()) {
                cursorX++;
            }
            else if (cursorY < (int)lines->size() - 1) {
                cursorY++;
                cursorX = 0;
            }
            return;
        }
        if (isUp) {
            if (cursorY > 0) {
                cursorY--;
                cursorX = std::min(cursorX, (int)(*lines)[cursorY].size());
            }
            return;
        }
        if (isDown) {
            if (cursorY < (int)lines->size() - 1) {
                cursorY++;
                cursorX = std::min(cursorX, (int)(*lines)[cursorY].size());
            }
            return;
        }
        if (mode == COMMAND) {
            if (key == '\r' || key == '\n' || key == 13) {
                isWriting = false;
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
                const std::string& currentLine = (*lines)[cursorY];
                size_t indentCount = 0;
                while (indentCount < currentLine.size() &&
                    (currentLine[indentCount] == ' ' || currentLine[indentCount] == '\t')) {
                    indentCount++;
                }
                std::string indent = currentLine.substr(0, indentCount);
                std::string remainder = currentLine.substr(cursorX);
                (*lines)[cursorY] = currentLine.substr(0, cursorX);
                lines->insert(lines->begin() + cursorY + 1, indent + remainder);

                cursorY++;
                cursorX = (int)indent.size();
                return;
            }
            if (key == '\t' || key == 9) {
                (*lines)[cursorY].insert(cursorX, "    ");
                cursorX += 4;
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

    void HandleInput(InputType input) override {
        if (focused && input == InputType::Enter) {
            isWriting = true;
        }
    }

    void Draw(std::ostream& buffer, int px, int py) override {
        if (!lines) return;
        int viewHeight = height - 1;
        if (cursorY < scroll) {
            scroll = cursorY;
        }
        else if (cursorY >= scroll + viewHeight) {
            scroll = cursorY - viewHeight + 1;
        }
        int maxScroll = std::max(0, (int)lines->size() - viewHeight);
        scroll = std::max(0, std::min(scroll, maxScroll));
        std::string bg = parent->Palette.Body;
        int totalLines = (int)lines->size();
        size_t lineNumberWidth = std::to_string(std::max(1, totalLines)).size() + 2;
        size_t textWidth = width - lineNumberWidth;
        if (cursorX < scrollX) {
            scrollX = cursorX;
        }
        else if (cursorX >= scrollX + textWidth) {
            scrollX = cursorX - textWidth + 1;
        }
        scrollX = std::max(0, scrollX);
        for (int i = 0; i < viewHeight; ++i) {
            int lineIndex = scroll + i;
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H" << bg;
            if (lineIndex < totalLines) {
                std::string lineNum = std::to_string(lineIndex + 1);
                buffer << "\033[38;2;120;120;120m";
                buffer << std::string(lineNumberWidth - lineNum.size(), ' ') << lineNum;
                buffer << bg;
                std::string visible;
                const std::string& fullLine = (*lines)[lineIndex];
                if ((int)fullLine.size() > scrollX) {
                    visible = fullLine.substr(scrollX);
                }
                else {
                    visible = "";
                }
                if (highligh) {
                    HighlightLine(buffer, visible, textWidth, bg);
                }
                else {
                    buffer << "\033[38;2;255;255;255m" << bg;
                    std::string toRender = visible.substr(0, textWidth);
                    buffer << toRender;
                    if ((int)toRender.size() < textWidth) {
                        buffer << std::string(textWidth - toRender.size(), ' ');
                    }
                }
                if (isWriting && lineIndex == cursorY) {
                    if (cursorX >= scrollX && cursorX < scrollX + textWidth) {
                        char ch = (cursorX < (int)fullLine.size())
                            ? fullLine[cursorX]
                            : ' ';
                        buffer << "\033[" << (py + y + i) << ";"
                            << (px + x + lineNumberWidth + (cursorX - scrollX)) << "H";
                        buffer << "\033[38;2;255;255;255;48;2;0;0;0m"
                            << ch << bg;
                    }
                }
            }
            else {
                buffer << "\033[38;2;80;80;80m" << std::string(lineNumberWidth - 1, ' ') << "~" << bg;
                buffer << std::string(textWidth, ' ');
            }
        }
        buffer << "\033[" << (py + y + viewHeight) << ";" << (px + x) << "H";
        if (isWriting) {
            buffer << (mode == INSERT ? "-- INSERT --" : "-- COMMAND --");
        }
        else {
            buffer << (focused ? "> [Press ENTER to edit]" : "  [Inactive]");
        }

        int statusLen = 1 + (isWriting ? (mode == INSERT ? 12 : 13) : 23);
        buffer << std::string(std::max(0, width - statusLen), ' ');
        buffer << "\033[0m";
    }
};