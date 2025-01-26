#include "Map/Room.h"

#include <Assets/SpriteSheet.h>

#include "Core/AssetManager.h"
#include "Utils/Constants.h"

Room::Room() : _position(sf::Vector2f(0, 0)) {
    initTiles();
}

Room::Room(sf::Vector2f position) : _position(position) {
    initTiles();
}

void Room::initTiles() {
    AssetManager* assetManager = AssetManager::getInstance();

    auto foamSprite = new SpriteSheet(SpriteSheet::SheetDescriptor {
        "../Data/Images/Map/Water/Foam.png",
        1,
        8,
        sf::Vector2i(-TILE_WIDTH, -TILE_HEIGHT),
        sf::IntRect(0, 0, TILE_WIDTH * 3, TILE_HEIGHT * 3),
    });
    sf::Texture* foamTexture = assetManager->loadTexture(foamSprite->getPath());


    auto rockBaseSprite = new SpriteSheet(SpriteSheet::SheetDescriptor {
        "../Data/Images/Map/Tilemap_Flat.png",
        1,
        1,
        sf::Vector2i(0, 0),
        sf::IntRect(TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT),
    });
    sf::Texture* rockBaseTexture = assetManager->loadTexture(rockBaseSprite->getPath());

    initTile(foamSprite, foamTexture, foamLayer);
    initTile(rockBaseSprite, rockBaseTexture, groundLayer);
}

void Room::initTile(SpriteSheet* baseSprite, sf::Texture* foamTexture, Tile* layer[ROOM_ROW_SIZE][ROOM_COL_SIZE]) {
    for (int i = 0; i < ROOM_ROW_SIZE; i++) {
        for (int j = 0; j < ROOM_COL_SIZE; j++) {
            Tile::TileDescriptor tileDescriptor;

            tileDescriptor.texture = foamTexture;
            tileDescriptor.spriteWidth = baseSprite->getRect().getSize().x;
            tileDescriptor.spriteHeight = baseSprite->getRect().getSize().y;
            tileDescriptor.position = sf::Vector2f(_position.x + i * TILE_WIDTH + baseSprite->getOffset().x,
                                                   _position.y + j * TILE_HEIGHT + baseSprite->getOffset().y);

            auto tile = new Tile();
            tile->init(tileDescriptor);

            layer[i][j] = tile;
        }
    }
}

Room::~Room() {
    for (int i = 0; i < ROOM_ROW_SIZE; i++) {
        for (int j = 0; j < ROOM_COL_SIZE; j++) {
            delete foamLayer[i][j];
        }
    }
}


void Room::render(sf::RenderWindow& window) {
    for (int i = 0; i < ROOM_ROW_SIZE; i++) {
        for (int j = 0; j < ROOM_COL_SIZE; j++) {
            foamLayer[i][j]->render(window);
        }
    }

    for (int i = 0; i < ROOM_ROW_SIZE; i++) {
        for (int j = 0; j < ROOM_COL_SIZE; j++) {
            groundLayer[i][j]->render(window);
        }
    }
}

void Room::update(float deltaMilliseconds) {
    for (int i = 0; i < ROOM_ROW_SIZE; i++) {
        for (int j = 0; j < ROOM_COL_SIZE; j++) {
            foamLayer[i][j]->update(deltaMilliseconds);
        }
    }
}
