#pragma once
#include "Core/AssetManager.h"
#include "Map/Tile.h"
#include "Utils/Constants.h"

namespace sf {
    class RenderWindow;
}

class Background {
    public:
        Background();
        Background(sf::Vector2f pos);
        ~Background();

    private:
        sf::Vector2f _position;
        Tile* backgroundTiles[BACKGROUND_ROW_SIZE][BACKGROUND_COL_SIZE];

        void initTiles();

    public:
        void render(sf::RenderWindow& window);
};
