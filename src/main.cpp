#include <cstdio>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>


int main() {
    // To-Do: Load game config from file instead of hardcoding values in code
    Game::GameCreateInfo gameCI;
    gameCI.gameTitle = GAME_TITLE;
    gameCI.screenWidth = 1920u;
    gameCI.screenHeight = 1080u;
    gameCI.frameRateLimit = 60u;

    Game game;
    const bool gameInitialized = game.init(gameCI);

    if (gameInitialized == false) {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;

    while (game.isRunning()) {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}
