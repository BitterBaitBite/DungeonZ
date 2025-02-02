#pragma once

class SpriteSheet {
    public:
        struct SpriteSheetInfo {
            const char* path;
            int rows;
            int cols;
            sf::Vector2i tileOffset;
            sf::IntRect renderRect;
        };

        SpriteSheet(const SpriteSheetInfo& spriteSheetInfo) :
            _rows(spriteSheetInfo.rows),
            _cols(spriteSheetInfo.cols),
            _path(spriteSheetInfo.path),
            _offset(spriteSheetInfo.tileOffset),
            _rect(spriteSheetInfo.renderRect) {}

        ~SpriteSheet() = default;

        int getRows() const { return _rows; }
        int getCols() const { return _cols; }
        const char* getPath() const { return _path; }
        sf::Vector2i getOffset() const { return _offset; }
        sf::IntRect getRect() const { return _rect; }

    private:
        int _rows;
        int _cols;
        const char* _path;
        sf::Vector2i _offset;
        sf::IntRect _rect;
};
