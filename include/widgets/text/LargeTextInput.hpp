#pragma once
#include <Widget.hpp>
#include <Window.hpp>
#include <userinput.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include "copyToClipboard.hpp"
#include <widgets/containers/VerticalContainer.hpp>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

class LargeTextInput : public Widget {
public:
    enum Mode { COMMAND, INSERT, VISUAL };
    std::vector<std::string>* lines;
    int cursorX = 0;
    int cursorY = 0;
    int scroll = 0;
    int scrollX = 0;
    int visualAnchorX = 0;
    int visualAnchorY = 0;
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

    bool IsInMultiLineComment(int targetLine) {
        if (!lines) return false;
        bool inComment = false;
        for (int i = 0; i <= targetLine && i < (int)lines->size(); ++i) {
            const std::string& line = (*lines)[i];
            for (size_t col = 0; col < line.size(); ++col) {
                if (!inComment && col + 1 < line.size() && line[col] == '/' && line[col + 1] == '*') {
                    inComment = true;
                    col++;
                }
                else if (inComment && col + 1 < line.size() && line[col] == '*' && line[col + 1] == '/') {
                    inComment = false;
                    col++;
                }
                else if (!inComment && col + 1 < line.size() && line[col] == '/' && line[col + 1] == '/') {
                    break;
                }
            }
        }
        return inComment;
    }

    void HighlightLine(std::ostream& buffer, const std::string& line, int width, const std::string& bg, bool& inMultiLineComment) {
        std::string kwColor = "\033[38;2;128;220;250m";
        std::string typeColor = "\033[38;2;53;132;215m";
        std::string customType = "\033[38;2;40;180;180m";
        std::string funcColor = "\033[38;2;220;220;130m";
        std::string strColor = "\033[38;2;206;145;120m";
        std::string numColor = "\033[38;2;181;206;168m";
        std::string preColor = "\033[38;2;197;134;192m";
        std::string opColor = "\033[38;2;212;212;212m";
        std::string varColor = "\033[38;2;156;220;254m";
        std::string commentColor = "\033[38;2;106;153;85m";

        std::vector<std::string> keywords = {
            "const", "static", "volatile", "mutable", "nullptr", "constexpr", "inline",
            "class", "struct", "union", "enum", "public", "private", "protected", "virtual",
            "override", "final", "friend", "explicit", "this", "operator", "typename", "template",
            "if", "else", "switch", "case", "default", "return", "for", "while", "do",
            "break", "continue", "goto", "try", "catch", "throw", "noexcept",
            "and", "or", "not", "new", "delete", "sizeof", "decltype", "typeid",
            "static_cast", "dynamic_cast", "const_cast", "reinterpret_cast",
            "using", "namespace", "import", "export", "module", "std", "true", "false"
        };

        std::vector<std::string> types = {
            "void", "std::nullptr_t", "bool", "char", "wchar_t",
            "char16_t", "char32_t", "short", "int",
            "long", "float", "double", "signed", "unsigned",
            "int8_t", "int16_t", "int32_t", "int64_t",
            "uint8_t", "uint16_t", "uint32_t", "uint64_t",
            "int_least8_t", "int_least16_t", "int_least32_t", "int_least64_t",
            "uint_least8_t", "uint_least16_t", "uint_least32_t", "uint_least64_t",
            "int_fast8_t", "int_fast16_t", "int_fast32_t", "int_fast64_t",
            "uint_fast8_t", "uint_fast16_t", "uint_fast32_t", "uint_fast64_t",
            "intmax_t", "uintmax_t", "size_t", "string", "vector"
        };

        std::string lastToken = "";
        bool expectingTypeName = false;

        for (int col = 0; col < width; ++col) {
            if (col >= (int)line.size()) {
                buffer << " ";
                continue;
            }
            bool highlighted = false;

            if (inMultiLineComment) {
                int end = col;
                while (end < (int)line.size()) {
                    if (end + 1 < (int)line.size() && line[end] == '*' && line[end + 1] == '/') {
                        end += 2;
                        inMultiLineComment = false;
                        break;
                    }
                    end++;
                }
                buffer << commentColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (col + 1 < (int)line.size() && line[col] == '/' && line[col + 1] == '/') {
                buffer << commentColor << line.substr(col) << bg;
                col = width - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (col + 1 < (int)line.size() && line[col] == '/' && line[col + 1] == '*') {
                inMultiLineComment = true;
                int end = col + 2;
                while (end < (int)line.size()) {
                    if (end + 1 < (int)line.size() && line[end] == '*' && line[end + 1] == '/') {
                        end += 2;
                        inMultiLineComment = false;
                        break;
                    }
                    end++;
                }
                buffer << commentColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (line[col] == '"' || line[col] == '\'') {
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
                int end = col + 1;
                while (end < (int)line.size() && (isalnum((unsigned char)line[end]) || line[end] == '_')) end++;
                buffer << preColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            else if (col + 1 < (int)line.size()) {
                std::string duo = line.substr(col, 2);
                if (duo == "<<" || duo == ">>" || duo == "::" || duo == "->" || duo == "==" || duo == "!=" || duo == "<=" || duo == ">=") {
                    buffer << opColor << duo << bg;
                    col += 1;
                    highlighted = true;
                    lastToken = "";
                    expectingTypeName = false;
                }
            }

            if (!highlighted && std::string("+-*/%=!<>|&;{},().[]").find(line[col]) != std::string::npos) {
                buffer << opColor << line[col] << bg;
                highlighted = true;
                if (line[col] == ';' || line[col] == '{' || line[col] == '}') {
                    lastToken = "";
                    expectingTypeName = false;
                }
            }

            if (!highlighted && isdigit((unsigned char)line[col]) && (col == 0 || !isalnum((unsigned char)line[col - 1]))) {
                int end = col;
                if (line[col] == '0' && col + 1 < (int)line.size() && (line[col + 1] == 'x' || line[col + 1] == 'X')) {
                    end += 2;
                    while (end < (int)line.size() && isxdigit((unsigned char)line[end])) end++;
                }
                else {
                    while (end < (int)line.size() && (isdigit((unsigned char)line[end]) || line[end] == '.' || line[end] == 'f' || line[end] == 'U' || line[end] == 'L')) end++;
                }
                buffer << numColor << line.substr(col, end - col) << bg;
                col = end - 1;
                highlighted = true;
                lastToken = "";
                expectingTypeName = false;
            }

            if (!highlighted && (isalpha((unsigned char)line[col]) || line[col] == '_')) {
                int start = col;
                int end = col;
                while (end < (int)line.size() && (isalnum((unsigned char)line[end]) || line[end] == '_')) {
                    end++;
                }
                std::string identifier = line.substr(start, end - start);

                bool isKw = std::find(keywords.begin(), keywords.end(), identifier) != keywords.end();
                bool isTy = std::find(types.begin(), types.end(), identifier) != types.end();

                if (expectingTypeName) {
                    knownTypes.insert(identifier);
                    buffer << customType << identifier << bg;
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
                else if (isTy) {
                    buffer << typeColor << identifier << bg;
                    lastToken = identifier;
                }
                else if (end < (int)line.size() && line[end] == '(') {
                    buffer << funcColor << identifier << bg;
                    lastToken = "";
                }
                else if (knownTypes.find(identifier) != knownTypes.end()) {
                    buffer << customType << identifier << bg;
                    lastToken = identifier;
                }
                else {
                    if (knownTypes.find(lastToken) != knownTypes.end() || std::find(types.begin(), types.end(), lastToken) != types.end()) {
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
            case 'v':
                mode = VISUAL;
                visualAnchorX = cursorX;
                visualAnchorY = cursorY;
                return;
            case 'i':
                mode = INSERT;
                return;
            case 'a':
                mode = INSERT;
                if (cursorX < (int)(*lines)[cursorY].size()) cursorX++;
                return;
            case 'A':
                mode = INSERT;
                cursorX = (int)(*lines)[cursorY].size();
                return;
            case 'h': if (cursorX > 0) cursorX--; return;
            case 'l': if (cursorX < (int)(*lines)[cursorY].size()) cursorX++; return;
            case 'k': if (cursorY > 0) cursorY--; return;
            case 'j': if (cursorY < (int)lines->size() - 1) cursorY++; return;
            case '0':
                cursorX = 0;
                return;
            case '^': {
                const std::string& line = (*lines)[cursorY];
                cursorX = 0;
                while (cursorX < (int)line.size() && (line[cursorX] == ' ' || line[cursorX] == '\t')) {
                    cursorX++;
                }
                return;
            }
            case '$':
                cursorX = std::max(0, (int)(*lines)[cursorY].size() - 1);
                return;
            case 'g':
                if (readKey() == 'g') {
                    cursorY = 0;
                    cursorX = 0;
                }
                return;
            case 'G':
                cursorY = (int)lines->size() - 1;
                cursorX = 0;
                return;
            case 'w': {
                const std::string& line = (*lines)[cursorY];
                while (cursorX < (int)line.size() && !isalnum(line[cursorX]) && line[cursorX] != '_') cursorX++;
                while (cursorX < (int)line.size() && (isalnum(line[cursorX]) || line[cursorX] == '_')) cursorX++;
                while (cursorX < (int)line.size() && line[cursorX] == ' ') cursorX++;
                if (cursorX >= (int)line.size() && cursorY < (int)lines->size() - 1) {
                    cursorY++;
                    cursorX = 0;
                }
                return;
            }
            case 'b': {
                if (cursorX == 0 && cursorY > 0) {
                    cursorY--;
                    cursorX = (*lines)[cursorY].size();
                }
                const std::string& line = (*lines)[cursorY];
                while (cursorX > 0 && line[cursorX - 1] == ' ') cursorX--;
                while (cursorX > 0 && (isalnum(line[cursorX - 1]) || line[cursorX - 1] == '_')) cursorX--;
                return;
            }
            case 'x':
                if (cursorX < (int)(*lines)[cursorY].size()) {
                    (*lines)[cursorY].erase(cursorX, 1);
                }
                return;
            case 'd':
                if (readKey() == 'd') {
                    copyToClipboard((*lines)[cursorY] + "\n");
                    lines->erase(lines->begin() + cursorY);
                    if (lines->empty()) lines->push_back("");
                    cursorY = std::max(0, std::min(cursorY, (int)lines->size() - 1));
                    cursorX = 0;
                }
                return;
            case 'y':
                if (readKey() == 'y') {
                    copyToClipboard((*lines)[cursorY] + "\n");
                }
                return;
            }
        }
        else if (mode == VISUAL) {
            if (key == 27) {
                mode = COMMAND;
                return;
            }
            switch (key) {
            case 'h': if (cursorX > 0) cursorX--; return;
            case 'l': if (cursorX < (int)(*lines)[cursorY].size()) cursorX++; return;
            case 'k': if (cursorY > 0) cursorY--; return;
            case 'j': if (cursorY < (int)lines->size() - 1) cursorY++; return;
            case '0': cursorX = 0; return;
            case '$': cursorX = std::max(0, (int)(*lines)[cursorY].size()); return;
            }
            int startY = std::min(visualAnchorY, cursorY);
            int endY = std::max(visualAnchorY, cursorY);
            int startX = (visualAnchorY < cursorY) ? visualAnchorX : (visualAnchorY > cursorY ? cursorX : std::min(visualAnchorX, cursorX));
            int endX = (visualAnchorY < cursorY) ? cursorX : (visualAnchorY > cursorY ? visualAnchorX : std::max(visualAnchorX, cursorX));
            if (key == 'y') {
                std::string selectedText = "";
                for (int y = startY; y <= endY; ++y) {
                    const std::string& line = (*lines)[y];
                    int selStart = (y == startY) ? startX : 0;
                    int selEnd = (y == endY) ? std::min(endX + 1, (int)line.size()) : (int)line.size();

                    if (selStart < (int)line.size()) {
                        selectedText += line.substr(selStart, selEnd - selStart);
                    }
                    if (y < endY) selectedText += "\n";
                }
                copyToClipboard(selectedText);
                mode = COMMAND;
                return;
            }
            if (key == 'd' || key == 'x') {
                std::string selectedText = "";
                for (int y = startY; y <= endY; ++y) {
                    const std::string& line = (*lines)[y];
                    int selStart = (y == startY) ? startX : 0;
                    int selEnd = (y == endY) ? std::min(endX + 1, (int)line.size()) : (int)line.size();

                    if (selStart < (int)line.size()) {
                        selectedText += line.substr(selStart, selEnd - selStart);
                    }
                    if (y < endY) selectedText += "\n";
                }
                copyToClipboard(selectedText);

                if (startY == endY) {
                    std::string& line = (*lines)[startY];
                    int deleteLen = std::min(endX + 1, (int)line.size()) - startX;
                    if (startX < (int)line.size() && deleteLen > 0) {
                        line.erase(startX, deleteLen);
                    }
                }
                else {
                    std::string startPrefix = (*lines)[startY].substr(0, startX);
                    std::string endSuffix = (endX + 1 < (int)(*lines)[endY].size()) ? (*lines)[endY].substr(endX + 1) : "";

                    (*lines)[startY] = startPrefix + endSuffix;
                    lines->erase(lines->begin() + startY + 1, lines->begin() + endY + 1);
                }

                if (lines->empty()) lines->push_back("");

                cursorY = startY;
                cursorX = startX;
                mode = COMMAND;
                return;
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

        bool inMultiLineComment = IsInMultiLineComment(scroll - 1);
        int selStartY = std::min(visualAnchorY, cursorY);
        int selEndY = std::max(visualAnchorY, cursorY);
        int selStartX = (visualAnchorY < cursorY) ? visualAnchorX : (visualAnchorY > cursorY ? cursorX : std::min(visualAnchorX, cursorX));
        int selEndX = (visualAnchorY < cursorY) ? cursorX : (visualAnchorY > cursorY ? visualAnchorX : std::max(visualAnchorX, cursorX));

        for (int i = 0; i < viewHeight; ++i) {
            int lineIndex = scroll + i;
            buffer << "\033[" << (py + y + i) << ";" << (px + x) << "H" << bg;

            if (lineIndex < totalLines) {
                std::string lineNum = std::to_string(lineIndex + 1);
                buffer << "\033[38;2;120;120;120m";
                buffer << std::string(lineNumberWidth - lineNum.size(), ' ') << lineNum;
                buffer << bg;

                const std::string& fullLine = (*lines)[lineIndex];

                if (mode == VISUAL) {
                    for (int col = 0; col < (int)textWidth; ++col) {
                        int realCol = scrollX + col;

                        if (realCol < (int)fullLine.size()) {
                            char ch = fullLine[realCol];
                            bool inVisualRange = (lineIndex > selStartY || (lineIndex == selStartY && realCol >= selStartX)) &&
                                (lineIndex < selEndY || (lineIndex == selEndY && realCol <= selEndX));

                            if (inVisualRange) {
                                buffer << "\033[7m" << ch << "\033[27m" << bg;
                            }
                            else {
                                buffer << "\033[38;2;255;255;255m" << ch;
                            }
                        }
                        else {
                            bool inVisualRange = (lineIndex >= selStartY && lineIndex < selEndY && realCol == (int)fullLine.size());
                            if (inVisualRange) {
                                buffer << "\033[7m \033[27m" << bg;
                            }
                            else {
                                buffer << " ";
                            }
                        }
                    }
                }
                else if (highligh) {
                    std::string visible = (int)fullLine.size() > scrollX ? fullLine.substr(scrollX) : "";
                    HighlightLine(buffer, visible, textWidth, bg, inMultiLineComment);
                }
                else {
                    std::string visible = (int)fullLine.size() > scrollX ? fullLine.substr(scrollX) : "";
                    buffer << "\033[38;2;255;255;255m" << bg;
                    std::string toRender = visible.substr(0, textWidth);
                    buffer << toRender;
                    if ((int)toRender.size() < textWidth) {
                        buffer << std::string(textWidth - toRender.size(), ' ');
                    }
                }

                if (isWriting && lineIndex == cursorY && mode != VISUAL) {
                    if (cursorX >= scrollX && cursorX < scrollX + textWidth) {
                        char ch = (cursorX < (int)fullLine.size()) ? fullLine[cursorX] : ' ';
                        buffer << "\033[" << (py + y + i) << ";"
                            << (px + x + lineNumberWidth + (cursorX - scrollX)) << "H";
                        buffer << "\033[38;2;255;255;255;48;2;0;0;0m" << ch << bg;
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
            if (mode == INSERT) buffer << "-- INSERT --";
            else if (mode == VISUAL) buffer << "-- VISUAL --";
            else buffer << "-- COMMAND --";
        }
        else {
            buffer << (focused ? "> [Press ENTER to edit]" : "  [Inactive]");
        }

        int statusLen = 1 + (isWriting ? (mode == INSERT ? 12 : (mode == VISUAL ? 12 : 13)) : 23);
        buffer << std::string(std::max(0, width - statusLen), ' ');
        buffer << "\033[0m";
    }
};