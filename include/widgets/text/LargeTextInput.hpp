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
    LargeTextInput(int x, int y, int h, int w, std::vector<std::string>* target, bool hi);
    LargeTextInput(int h, int w, std::vector<std::string>* target, bool hi);
    int GetWidth() const override;
    int GetHeight() const override;
    bool IsInMultiLineComment(int targetLine);
    void HighlightLine(std::ostream& buffer, const std::string& line, int width, const std::string& bg, bool& inMultiLineComment);
    bool WantsRawInput() override;
    void HandleRawInput() override;
    void HandleInput(InputType input) override;
    void Draw(std::ostream& buffer, int px, int py) override;
};