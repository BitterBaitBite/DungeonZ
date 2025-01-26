#include <Assets/SpriteSheet.h>

#include "Map/Background.h"
#include "Core/AssetManager.h"
#include "Utils/Constants.h"

Background::Background() : _position(sf::Vector2f(0, 0)) {
    initTiles();
}

Background::Background(sf::Vector2f position) : _position(position) {
    initTiles();
}

void Background::initTiles() {
    AssetManager* assetManager = AssetManager::getInstance();

    SpriteSheet::SheetDescriptor baseSpriteDesc;
    baseSpriteDesc.path = "../Data/Images/Map/Water/Water.png";
    baseSpriteDesc.rows = 1;
    baseSpriteDesc.cols = 1;
    auto baseSprite = new SpriteSheet(baseSpriteDesc);

    sf::Texture* baseTexture = assetManager->loadTexture(baseSprite->getPath());

    for (int i = 0; i < BACKGROUND_ROW_SIZE; i++) {
        for (int j = 0; j < BACKGROUND_COL_SIZE; j++) {
            Tile::TileDescriptor tileDescriptor;

            tileDescriptor.position = sf::Vector2f(_position.x + i * TILE_WIDTH, _position.y + j * TILE_HEIGHT);
            tileDescriptor.texture = baseTexture;
            tileDescriptor.spriteWidth = TILE_WIDTH;
            tileDescriptor.spriteHeight = TILE_HEIGHT;

            auto tile = new Tile();
            tile->init(tileDescriptor);

            backgroundTiles[i][j] = tile;
        }
    }
}

Background::~Background() {
    for (int i = 0; i < BACKGROUND_ROW_SIZE; i++) {
        for (int j = 0; j < BACKGROUND_COL_SIZE; j++) {
            delete backgroundTiles[i][j];
        }
    }
}


void Background::render(sf::RenderWindow& window) {
    for (int i = 0; i < BACKGROUND_ROW_SIZE; i++) {
        for (int j = 0; j < BACKGROUND_COL_SIZE; j++) {
            backgroundTiles[i][j]->render(window);
        }
    }
}
