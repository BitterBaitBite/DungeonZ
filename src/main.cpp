#include <cstdio>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>

#ifndef NDEBUG
#define DEBUG_MODE
#endif

int main() {
    Game game;
    const bool gameInitialized = game.init();

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
