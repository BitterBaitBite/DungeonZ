#pragma once

#include <cstdint>

class Enemy;

namespace sf
{
	class RenderWindow;
}

class World
{
	public:

		~World();

		// TO-DO: Ideally the scene should be read from file.
		bool load();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		// This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
		Enemy* m_enemy{ nullptr };
};