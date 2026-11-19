#pragma once
#include "Widget.hpp"
#include <vector>
#include <string>

namespace litw {

    class Table : public Widget {
    public:
        std::vector<std::string> headers;
        std::vector<std::vector<std::string>> rows;
        std::vector<int> colWidths;
        Table(int x, int y, std::vector<std::string> hdrs, std::vector<int> widths);
        Table(std::vector<std::string> hdrs, std::vector<int> widths);
        void AddRow(std::vector<std::string> row);
        void Draw(std::ostream& buffer, int px, int py) override;
        int GetWidth() const override;
        int GetHeight() const override;
    private:
        void DrawLine(std::ostream& buf, int xPos, int yPos, const char* left, const char* mid, const char* right, const char* fill);
        void DrawDataRow(std::ostream& buf, int xPos, int yPos, const std::vector<std::string>& data);
    };

}