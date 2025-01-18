#pragma once
#include <string>

#include "SFML/System/Vector2.hpp"

class SpriteSheet {
    public:
        struct SheetDescriptor {
            const char* path;
            int rows;
            int cols;
        };

        SpriteSheet() {};

        void init(const SheetDescriptor& sheet) { _sheet = sheet; }

        int getRows() const { return _sheet.rows; }
        int getCols() const { return _sheet.cols; }
        const char* getPath() const { return _sheet.path; }

    private:
        SheetDescriptor _sheet;
};
