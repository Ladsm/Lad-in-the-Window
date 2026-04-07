#pragma once
#include "Widget.hpp"
#include <vector>
#include <string>

class Table : public Widget {
public:
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    std::vector<int> colWidths;
    Table(int x, int y, std::vector<std::string> hdrs, std::vector<int> widths)
        : headers(hdrs), colWidths(widths) {
        this->x = x; this->y = y;
        this->focusable = false;
    }
    void AddRow(std::vector<std::string> row) { rows.push_back(row); }
    void Draw(std::ostream& buffer, int px, int py) override {
        buffer << "\033[38;2;0;0;0;48;2;192;192;192m";
        int curY = py + y;
        DrawLine(buffer, px + x, curY++, "┌", "┬", "┐", "─");
        DrawDataRow(buffer, px + x, curY++, headers);
        DrawLine(buffer, px + x, curY++, "├", "┼", "┤", "─");
        for (const auto& row : rows) {
            DrawDataRow(buffer, px + x, curY++, row);
        }
        DrawLine(buffer, px + x, curY, "└", "┴", "┘", "─");
    }
    int GetWidth() const override {
        int total = 0;
        for (auto w : colWidths) total += w;
        total += (int)colWidths.size() + 1;
        return total;
    }
    int GetHeight() const override {
        return 4 + (int)rows.size();
    }
private:
    void DrawLine(std::ostream& buf, int xPos, int yPos, const char* left, const char* mid, const char* right, const char* fill) {
        buf << "\033[" << yPos << ";" << xPos << "H" << left;
        for (size_t i = 0; i < colWidths.size(); ++i) {
            for (int j = 0; j < colWidths[i]; ++j) buf << fill;
            if (i < colWidths.size() - 1) buf << mid;
        }
        buf << right;
    }
    void DrawDataRow(std::ostream& buf, int xPos, int yPos, const std::vector<std::string>& data) {
        buf << "\033[" << yPos << ";" << xPos << "H│";
        for (size_t i = 0; i < colWidths.size(); ++i) {
            std::string cell = (i < data.size()) ? data[i] : "";
            if (cell.length() > (size_t)colWidths[i]) cell = cell.substr(0, colWidths[i]);
            buf << cell;
            for (size_t j = cell.length(); j < (size_t)colWidths[i]; ++j) buf << " ";
            buf << "│";
        }
    }
};
