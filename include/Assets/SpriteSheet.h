#pragma once

#include "sfml\Graphics.hpp"

class SpriteSheet {
    public:
        struct SheetDescriptor {
            const char* path;
            int rows;
            int cols;
            sf::Vector2i tileOffset;
            sf::IntRect renderRect;
        };

        SpriteSheet(SheetDescriptor descriptor) : _path(descriptor.path), _rows(descriptor.rows), _cols(descriptor.cols), _offset(descriptor.tileOffset), _rect(descriptor.renderRect) {};
        ~SpriteSheet() { delete _path; _path = nullptr; };

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
