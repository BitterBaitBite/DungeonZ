#pragma once

#include <tmxlite/Map.hpp>

#include "Assets/SpriteSheet.h"
#include "Gameplay/Player.h"
#include "Map/Dungeon.h"

class Background;
class Room;

namespace sf {
    class RenderWindow;
}

class MapLayer;

class World {
    public:
        ~World();

        bool load();
        void update(uint32_t deltaMilliseconds);
        void render(sf::RenderWindow& window);
        void reset();
        void getPlayerSpriteInfo(SpriteSheet*& playerSpriteSheet, sf::Texture*& playerTexture);
        void getPlayerFireSpriteInfo(SpriteSheet*& fireSpriteSheet, sf::Texture*& fireTexture);
        void setPlayerInfo(SpriteSheet* playerSpriteSheet, sf::Texture* playerTexture, SpriteSheet* fireSpriteSheet,
                           sf::Texture* fireTexture, Player::PlayerInfo& playerInfo);
        void getPlayerInfo(Player::PlayerInfo& playerInfo);

    private:
        Dungeon* _currentDungeon { nullptr }; // Dungeon = Level

        bool loadPlayer();
};
